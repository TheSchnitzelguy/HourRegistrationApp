#include "databasehandler.h"


databaseHandler::databaseHandler(QString x)
{
    databaseLocation = x;
    initializeDatabase();
}

/*!
*@brief Function that initializes a database connection that is passed by the constructor
*@param None*/
void databaseHandler::initializeDatabase()
{
    bool existingTables = false;
    /*initialize and create database*/    
    qDebug() << "db connection initializing...";
    QSqlDatabase hourDb;
    hourDb = QSqlDatabase::addDatabase("QSQLITE");
    //QString dbPath = "../db/db.sqlite";
    hourDb.setDatabaseName(databaseLocation);
    qDebug() << "opening database: " << databaseLocation;

    //first, check if connection with SQL driver can be established
    if(hourDb.open())
    {
        qDebug() << "Database opened!" ;


        if(checkForExistingTables(hourDb) == true)
        {
            existingTables = true;
            qDebug() << "Existing tables found! existingTables = " << existingTables;
            QDate testDate = QDate::fromString("20-11-1962");
            /*TODO: fix borked datatypes*/
            insertJob(testDate, QTime::fromString("11"), QTime::fromString("9:00") , QTime::fromString("18:00"), "blahblah", true);
        }
        else
        {
           createNewDatabase(hourDb);
           existingTables = false;
        }
    }
    else if(!hourDb.open())
    {
        QMessageBox dbNotLoaded;
        dbNotLoaded.critical(0, "Error", "Database could not be opened!");
        qDebug() << "Error: database could not be opened: " << hourDb.lastError();

    #ifndef DEBUG
         // If database could not be laoded, kill application and return error
        QCoreApplication::exit(1);
    #endif
    }

}

bool databaseHandler::createNewDatabase(QSqlDatabase db)
{
    QSqlQuery dbQuery;

    QString dateTable = "CREATE TABLE Dates("
                        "jobID integer NOT NULL PRIMARY KEY AUTOINCREMENT,"
                        "anoID integer NOT NULL,"
                        "timeID integer NOT NULL,"
                        "date TEXT(10),"
                        "year TEXT(4),"
                        "FOREIGN KEY (anoID)"
                            "REFERENCES Annotations (anoID),"
                        "FOREIGN KEY (timeID)"
                            "REFERENCES Times (timeID));";

    QString annotationTable = " CREATE TABLE Annotations("
                              "anoID integer NOT NULL PRIMARY KEY AUTOINCREMENT,"
                              "annotation VARCHAR(128));";

    QString timesTable = " CREATE TABLE Times("
                          "timeID integer NOT NULL PRIMARY KEY AUTOINCREMENT,"
                          "workedHours TEXT(5),"
                          "startingTime TEXT(5),"
                          "endingTime TEXT(5),"
                          "pause BOOLEAN);";


#ifdef DEBUG
    qDebug() << "Inserting the following queries into database: \n" << dateTable << annotationTable << timesTable;
#endif

//TODO: fix this

    if (dbQuery.exec(dateTable) && dbQuery.exec(annotationTable) && dbQuery.exec(timesTable) == true)
    {
         qDebug() << "Succesfully created database tables!";
         return true;
    }
    else
    {
        qDebug() << "Error, tables could not be inserted!" << dbQuery.lastError();
        return false;
    }


    db.close();
}

void databaseHandler::removeDatabase(QSqlDatabase db)
{
    QSqlQuery dbQuery;
    QString emptyDates = "DROP TABLE IF EXISTS Dates;";
    QString emptyAnnotations = " DROP TABLE IF EXISTS Annotations;";
    QString emptyTimes = " DROP TABLE IF EXISTS Times;";


    if (dbQuery.exec(emptyDates) && dbQuery.exec(emptyAnnotations) && dbQuery.exec(emptyTimes) == true)
    {
        qDebug() << "Running statements: " << emptyDates << emptyAnnotations << emptyTimes;
        qDebug() << "Successfully deleted all tables from database!" ;
    }
    else
    {
        qDebug() << "Error! could not clear database. Reason:" << dbQuery.lastError();
    }

    db.close();
}


void databaseHandler::insertJob(QDate jobDate, QTime workedHours, QTime startTime, QTime endTime, QString annotation, bool pause)
{
    int year = jobDate.year();
    QString formattedJobDate = jobDate.toString("MM-dd");

qDebug() << "year contains: " << year << " formattedJobDate contains: " << formattedJobDate;
//    QSqlQuery dateQuery, annotationQuery, timesQuery;

//    /*date table*/
//    dateQuery.prepare("INSERT INTO Dates ("
//                      "date,"
//                      "year)"
//                      "VALUES (?,?);");

//    dateQuery.addBindValue(formattedJobDate);
//    dateQuery.addBindValue(year);

//    /*annotations table*/
//    annotationQuery.prepare("INSERT INTO Annotations ("
//                            "annotation)"
//                            "VALUES (?);");
//    annotationQuery.addBindValue(annotation);

//    /*times table*/
//    timesQuery.prepare("INSERT INTO Times ("
//                       "workedHours,"
//                       "startingTime,"
//                       "endingTime,"
//                       "pause)"
//                       "VALUES (?,?,?,?);");

//    timesQuery.addBindValue(workedHours);
//    timesQuery.addBindValue(startTime);
//    timesQuery.addBindValue(endTime);
//    timesQuery.addBindValue(pause);

}

//helper functions
/*--------------------------------------------------------------------------------------------------------------*/

bool databaseHandler::checkJob()
{
    return true;
}


bool databaseHandler::checkForExistingTables(QSqlDatabase db)
{

    if (db.tables().contains(QLatin1String("Dates")) && db.tables().contains(QLatin1String("Annotations")) && db.tables().contains(QLatin1String("Times")))
    {
        return true;
    }
    else
    {
        return false;
    }
}







