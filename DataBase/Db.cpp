//
// Created by zeyad on 11/18/16.
//
#include "Db.h"
#include "iostream"
using namespace std;

// callback will be used in select * instructions
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

Db::Db()
{
    sqlite3 *db;
    int rc;
    char * sql;
    char *zErrMsg = 0;

/* Opening the database ourSystem.db
 * If you want to create a new database
 * on the terminal type $sqlite3 your database name.db
 * */
    if (sqlite3_open("newDb.db", &db) == SQLITE_OK)
        cout << "Opened db successfully\n";
    else
        cout << "Failed to open db\n";


// Create table user
    sql = "CREATE TABLE user("  \
         "Token INT PRIMARY KEY     NOT NULL," \
         "Name           TEXT    NOT NULL," \
         "Password         INT  NOT NULL );";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        cout<<"SQL error in user table:"<<zErrMsg<<endl;
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        cout<<"Table user created successfully"<<endl;
        //fprintf(stdout, "Table created successfully\n");
    }

//create table imageList
    sql = "CREATE TABLE imageList("  \
         "ImageName char(100) PRIMARY KEY     NOT NULL," \
         "OwnerAddr           INT    NOT NULL," \
         "OwnerPort         INT  NOT NULL );";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        cout<<"SQL error in imageList table:"<<zErrMsg<<endl;
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        cout<<"Table imageList created successfully"<<endl;
        //fprintf(stdout, "Table created successfully\n");
    }


//create table viewBy
    sql = "CREATE TABLE viewBy("  \
         "viewId INT PRIMARY KEY     NOT NULL," \
         "ImageNameFK          char(100)    NOT NULL," \
         "TokenFK         INT  NOT NULL, "\
         "FOREIGN KEY (ImageNameFK) REFERENCES imageList(ImageName),"\
         "FOREIGN KEY (TokenFK) REFERENCES user(Token) );";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        cout<<"SQL error in viewBy table:"<<zErrMsg<<endl;
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        cout<<"Table viewBy created successfully"<<endl;
        //fprintf(stdout, "Table created successfully\n");
    }



    //close database
    sqlite3_close(db);


}



