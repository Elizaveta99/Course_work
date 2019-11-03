#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wfdb/wfdb.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char * argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	//char sql[500] = "";

	//rc = sqlite3_open("C:\\Users\\User\\Desktop\\1studing\\Курсач 3 курс\\wfdb\\ECG_database.db", &db);
	rc = sqlite3_open("ECG_database.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	int recordNumber = 230, id = 45;
	for (int i = 0; i < 5; i++) 
	{
		char sql[500] = "";
		/* Create SQL statement */
		//sql = "INSERT INTO Records (id,name) VALUES (4, 'mitdb/102-0' ); ";

		char nameString[4], idString[3];
		//itoa(recordNumber, nameString, 10);
		//itoa(id, idString, 10);

		snprintf(nameString, sizeof(nameString), "%d", recordNumber);
		snprintf(idString, sizeof(idString), "%d", id);

		printf("%s\n", nameString);
		printf("%s\n", idString);

		//nameString[strlen(nameString) - 1] = '\0';
		//idString[strlen(idString) - 1] = '\0';

		

		strcat(sql, "INSERT INTO Records (id,name) VALUES (");
		//sql = "INSERT INTO Records (id,name) VALUES (";

	
		strcat(sql, idString);
		strcat(sql, ", 'mitdb/");
		strcat(sql, nameString);
		strcat(sql, "');");


		printf("%s\n", sql);
		//return 0;

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Records created successfully\n");
		}
		recordNumber++;
		id++;
	}

	sqlite3_close(db);


	WFDB_Anninfo a;
	WFDB_Annotation annot;

	a.name = "atr"; a.stat = WFDB_READ;
	/*if (annopen("100s", &a, 1) < 0)
		exit(1);
	while (getann(0, &annot) == 0)
		printf("%s %s\n", mstimstr(annot.time), annstr(annot.anntyp));*/

	if (annopen(argv[1], &a, 1) < 0)
		exit(1);
	int cnt = 0, cntN = 0;
	while (getann(0, &annot) == 0)
	{
		cnt++;
		if (strcmp(annstr(annot.anntyp), "N") != 0)
			//cntN++;
			printf("%s %s\n", mstimstr(annot.time), annstr(annot.anntyp));
	}
	printf("%d %d\n", cnt, cntN);

	exit(0);
}