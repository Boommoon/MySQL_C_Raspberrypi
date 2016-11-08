#include <stdio.h>
#include <stdlib.h>

//==================================================================
//======================這是一定要include的=========================
//==================================================================
#include <string>
#include <iostream>
#include <mysql/mysql.h> 
//請記得安裝libmysqlclient-dev
//sudo apt-get -y install libmysqlclient-dev
//編譯程式
//g++ -o XXXX XXX.c `mysql_config --cflags` `mysql_config --libs`
//==================================================================

#define DATABASE_NAME  "Database" //資料庫名稱
#define DATABASE_USERNAME "Test"  //mysql帳號
#define DATABASE_PASSWORD "1234"//mysql密碼

#define IPaddress "192.168.1.1"//mysql_ip

using namespace std;
MYSQL *mysql1;

//==================================================================
//=====================用字串來儲存mysql指令========================
//==================================================================
string s_mysql_command;
string s_mysql_Front = "INSERT INTO Table ( Name ) VALUES ( '";
string s_mysql_Back = "' )";
//==================================================================
char c_str[256];

int i_temp[10]={10,20,30,40,50,60,70,80,90,100};

//==================================================================
//=============================連結資料庫===========================
//==================================================================
void mysql_connect (void)
{
  //initialize MYSQL object for connections
  mysql1 = mysql_init(NULL);
  
  if(mysql1 == NULL)
  {
    fprintf(stderr, "ABB : %s\n", mysql_error(mysql1));
    return;
  }
  //Connect to the database
  if(mysql_real_connect(mysql1, IPaddress, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME, 0, NULL, 0) == NULL)
  {
    fprintf(stderr, "%s\n", mysql_error(mysql1));
  }
  else
  {
    printf("Database connection successful.\r\n");
  }
}

//==================================================================
//==========================取消連結資料庫==========================
//==================================================================
void mysql_disconnect (void)
{
  mysql_close(mysql1);
  printf( "Disconnected from database.\r\n");
}

//==================================================================
//==========================寫入資料庫==============================
//==================================================================
void mysql_write_something (void)
{
  sprintf( c_str, "%i", i_temp[5] );
  s_mysql_command =s_mysql_Front + c_str + s_mysql_Back;
  if(mysql1 != NULL)
  {
    //Retrieve all data from alarm_times
    if (mysql_query(mysql1,s_mysql_command.c_str()))
    { 
      fprintf(stderr, "%s\n", mysql_error(mysql1));
      return;
    }
  }
}


int main(int argc, char **argv)
{
  mysql_connect();
  mysql_write_something ();
  
  if (mysql1 != NULL)
  {
    if (!mysql_query(mysql1, "SELECT Number, Name FROM Table WHERE Number = 99"))
    {
      MYSQL_RES *result = mysql_store_result(mysql1);
      if (result != NULL)
      {
        //Get the number of columns
        int num_rows = mysql_num_rows(result);
        int num_fields = mysql_num_fields(result);
        
        MYSQL_ROW row;   //An array of strings
        while( (row = mysql_fetch_row(result)) )
        {
          if(num_fields >= 2)
          {
            char *value_int = row[0];
            char *value_string = row[1];
            
            printf( "Got value %s\n", value_string);
          }
        }
        mysql_free_result(result);
      }
      else
      {
        printf("Cannot get data\r\n");
      }
    }
    else
    {
      printf("Cannot SELECT\r\n");
    }
  }
  mysql_disconnect();
  return 0;
}