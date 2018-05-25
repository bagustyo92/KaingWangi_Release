#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <libpq-fe.h>
#include <sqlite3.h>

#include "elm_membercard.h"
#include "elm.h"


static sqlite3 *membercard_log = NULL;
	PGconn *conn;

gboolean get_card_member_data (const char *cmd)
{
	GChecksum *cksum;
	PGresult *res;
	int status, row, col, i, j, ret;

	if (!elm_db_connect (&conn))
	{	
		elm_debug (ELM_DEBUG_WARNING, "%s: Invalid Connect to Server \n", __func__);
			
		membercard[0][0]=0;
		return FALSE;
	}
	
	res = PQexec (conn, cmd);
	status = PQresultStatus (res);
	if (status != PGRES_TUPLES_OK)
	{
		elm_debug (ELM_DEBUG_ERROR, "%s: %s", __func__, 
					PQresultErrorMessage (res));
					
		membercard[0][0]=0;
		elm_db_close (&conn);
		return FALSE;
	}
	row = PQntuples (res);
	col = PQnfields (res);

	if ((row == 1)&&(col==3))
	{
		elm_debug (ELM_DEBUG_WARNING, "%s: Get Data From Server\n", __func__);
		printf("UID              Card Number      Saldo\n");     
		for(int i=0;i<row;i++)
		{
		membercard[i][0] = strtoull (PQgetvalue (res, i, 0), NULL, 10);
		membercard[i][1] = strtoull (PQgetvalue (res, i, 1), NULL, 10);
		membercard[i][2] = strtoull (PQgetvalue (res, i, 2), NULL, 10);
		printf("%16llu %16llu %16llu\n", membercard[i+1][0],membercard[i][1],membercard[i][2]); 
		}
		PQclear (res);
		elm_db_close (&conn);
		return TRUE;
	}else if (row==0)
	{
		elm_debug (ELM_DEBUG_WARNING, "%s: invalid row or col number: %s %d %d\n", __func__, cmd,col,row);
		PQclear (res);
		elm_db_close (&conn);
		membercard[0][0]=1;
		return FALSE;
	}

}


gboolean find_card_member_uid (unsigned long long  uid)
{
	    char label[100];
        sprintf(label,"select * from enco_get_membercard('%010llu')",uid);

     if((get_card_member_data(label)))
	{	
		return TRUE;
	} else if (membercard[0][0]==1)
	{
		elm_debug (ELM_DEBUG_ERROR, "%s: cant find %llu\n", __func__, uid);
		return FALSE;
	} else
	{
		return FALSE;
	}
}

gboolean card_member_update (unsigned long long uid, unsigned long long saldo)
{
	GChecksum *cksum;
	PGconn *conn;
	PGresult *res;
	int status, row, col, i, j;

	// card_member_update_file(uid,saldo);
	
	
	elm_debug (ELM_DEBUG_INFO, "%s: Member Card Update (%llu,%llu)\n", __func__,saldo, uid);
	char *cmd;
	cmd = g_strdup_printf("select enco_update_membercard (%llu,'%llu')",saldo,uid);
	
	if (!elm_db_connect (&conn))
		return FALSE;
		
	res = PQexec (conn, cmd);
	status = PQresultStatus (res);
	if (status != PGRES_TUPLES_OK)
		{
			elm_debug (ELM_DEBUG_ERROR, "%s: %s %s\n", __func__, cmd,
								 PQresultErrorMessage (res));
			elm_db_close (&conn);
			return FALSE;
		}

		PQclear (res);
	elm_db_close (&conn);
	return TRUE;
}


gboolean card_member_update_file (unsigned long long uid, unsigned long long  saldo)
{
	char *cmd;
	int ret;
  
	ret = sqlite3_open (ELM_MEMBERCARD_FILE, &membercard_log);
	if (ret != SQLITE_OK)
	{
      elm_debug (ELM_DEBUG_FATAL, "%s: could not open buffer\n", __func__);
      return FALSE;
    }

	cmd = sqlite3_mprintf ("UPDATE tblmembercard SET saldo=%llu WHERE uid='%llu'",saldo,uid );
 	
	elm_debug (ELM_DEBUG_INFO, "%s: Member Card Update (%llu,%llu)\n", __func__,saldo,uid);
				
	ret = sqlite3_exec (membercard_log, cmd, NULL, NULL, NULL);
	sqlite3_free (cmd);
	if (ret != SQLITE_OK)
    {
      elm_debug (ELM_DEBUG_FATAL, "%s: %s\n", __func__,
                 sqlite3_errmsg (membercard_log));
      return FALSE;
    }
    
    sqlite3_close (membercard_log);
    
    
  return TRUE;
}

gboolean card_member_insert_file (int baris)
{
  char *cmd;
  int ret;
  
   cmd = sqlite3_mprintf ("DELETE FROM mc WHERE uid=%llu",membercard[baris][0] );	
	ret = sqlite3_exec (membercard_log, cmd, NULL, NULL, NULL);
	sqlite3_free (cmd);
	if (ret != SQLITE_OK)
    {
      elm_debug (ELM_DEBUG_FATAL, "%s: %s\n", __func__,
                 sqlite3_errmsg (membercard_log));
    }
  
  
  cmd = sqlite3_mprintf ("INSERT INTO mc VALUES(%llu,'',%llu,%llu)",membercard[baris][0],membercard[baris][1],membercard[baris][2]);	
  ret = sqlite3_exec (membercard_log, cmd, NULL, NULL, NULL);
  sqlite3_free (cmd);
  if (ret != SQLITE_OK)
    {
      elm_debug (ELM_DEBUG_FATAL, "%s: %s\n", __func__,
                 sqlite3_errmsg (membercard_log));
    }
    
  return TRUE;
}

gboolean get_card_member_data_file ()
{
	  char **table,*cmd;
	  int buffer, ret, row, col, i, j;
	  
	  cmd = g_strdup_printf ("SELECT * FROM mc");
      ret = sqlite3_get_table (membercard_log, cmd, &table, &row, &col, NULL);
      g_free (cmd);
      if (ret != SQLITE_OK)
        {
          elm_debug (ELM_DEBUG_FATAL, "%s: %s\n", __func__, 
                     sqlite3_errmsg (membercard_log));
          return FALSE;
        }
		
		elm_debug (ELM_DEBUG_WARNING, "%s: Update Member Card From File \n", __func__);
		printf("UID              Card Number      Saldo\n");     
		for (i = 1; i <= row; i++)
        {
			membercard[i][0]=strtoull(table[(i*4)+0],NULL,10);
			membercard[i][1]=strtoull(table[(i*4)+2],NULL,10);
			membercard[i][2]=strtoull(table[(i*4)+3],NULL,10);
			printf("%16llu %16llu %16llu\n", membercard[i][0],membercard[i][1],membercard[i][2]);     
		}
		
	return TRUE;
}


