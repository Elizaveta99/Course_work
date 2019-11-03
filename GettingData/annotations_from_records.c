#include <stdio.h>
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
		{
			cntN++;
		}

		printf("%s %s\n", mstimstr(annot.time), annstr(annot.anntyp));
	}
	printf("%d %d\n", cnt, cntN);

	exit(0);
}