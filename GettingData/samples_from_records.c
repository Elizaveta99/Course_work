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

	//signals
	int i, j, nsig;
	WFDB_Sample *v;
	WFDB_Siginfo *s;

	nsig = isigopen(argv[1], NULL, 0);
	if (nsig < 1)
		exit(1);
	s = (WFDB_Siginfo *)malloc(nsig * sizeof(WFDB_Siginfo));
	if (isigopen(argv[1], s, nsig) != nsig)
		exit(1);
	v = (WFDB_Sample *)malloc(nsig * sizeof(WFDB_Sample));
	for (i = 0; i < 360*3; i++) {
		if (getvec(v) < 0)
			break;
		for (j = 0; j < nsig; j++)
			printf("%8d", v[j]);
		printf("\n");
	}
	printf("\n");

	a.name = "atr"; a.stat = WFDB_READ;
	if (annopen(argv[1], &a, 1) < 0) // открыла файл аннотация для записи
		exit(1);

	int cnt = 0, cntN = 0;
	while (getann(0, &annot) == 0) // читаю очередную аннот из файла аннот
	{
		//printf("annot.aux = %s!\n beat_type = %s\n sample(time) = %d\n", annot.aux, annstr(annot.anntyp), annot.time);
//		printf("annot.aux = %s!\n beat_type = %s\n sample(time) = %s\n", annot.aux, annstr(annot.anntyp), mstimstr(annot.time));

		cnt++;
		if (strcmp(annstr(annot.anntyp), "L") == 0)
		{
			cntN++;
			printf("annot.aux = %s!\n beat_type = %s\n sample(time) = %d, time(in ms) = %s\n", annot.aux, annstr(annot.anntyp), annot.time, mstimstr(annot.time));

		}

		//printf("%s %s\n", mstimstr(annot.time), annstr(annot.anntyp));
	}
	printf("%d %d\n", cnt, cntN);

	exit(0);
}