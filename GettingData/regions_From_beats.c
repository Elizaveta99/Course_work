#include <stdio.h>
#include <stdlib.h>
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
	sqlite3_stmt *res;

	char* sql;


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
	if (annopen(argv[1], &a, 1) < 0)
		exit(1);

	int start[2000];
	int finish[2000];
	int regtype_id[2000];
	int cnt_reg = 0;
	int finish_prev = -1;

	int cnt = 0, cntN = 0;

	int flagBegin = -1;

	while (getann(0, &annot) == 0)
	{
		cnt++;

		char *rhythm;
		rhythm = malloc(50);
		sprintf(rhythm, annstr(annot.anntyp));

		printf("annot.aux = %s!\n rhythm = %s!\n rhythm_size = %d\n beat_type = %s\n sample(time) = %d\n", annot.aux, rhythm, strlen(rhythm), annstr(annot.anntyp), annot.time);

		if (strcmp(rhythm, "V") == 0 && flagBegin == -1) // (+RBBB, +V if there are in this record)
		{
			flagBegin = 1;
			if (cnt_reg == 0)
			{
				start[cnt_reg] = annot.time;
			}
			else
			{
				start[cnt_reg] = annot.time;
				finish[cnt_reg - 1] = finish_prev;
			}
			cnt_reg++;

			//printf("annot.aux = %s!\n rhythm = %s!\n rhythm_size = %d\n beat_type = %s\n sample(time) = %d\n", annot.aux, rhythm, strlen(rhythm), annstr(annot.anntyp), annot.time);

		}
		else
			if (strcmp(rhythm, "V") == 0 && flagBegin != -1)
			{
				finish_prev = annot.time;
			}
			else
				if (strcmp(rhythm, "V") != 0)
				{
					//finish_prev = annot.time;
					flagBegin = -1;	// previous L-region efinished, the new one is beginning
				}

		if (strcmp(annstr(annot.anntyp), "N") != 0)
		{
			cntN++;
		}

		free(rhythm);
	}
	finish[cnt_reg - 1] = finish_prev; // правильно ли, если не на L закончился?  // finish time for last region

	printf("\nreg_amount = %d\n", cnt_reg);

	int new_id = 2720;                                    // record 24

	for (int i = 0; i < cnt_reg; i++)
	{
		printf("type = %d\n start = %d\n finish = %d\n", 18, start[i], finish[i]);

		if (finish[i] != -1 && finish[i] > start[i])  // only regions from several beats, not isolated
		{
			sql = malloc(500);
			new_id++;
			//printf("BeatType_id = %d new_id = %d\n", BeatType_id, new_id); // 
			sprintf(sql, "INSERT INTO Regions VALUES(%d, %d, %d, %d, %d)", new_id, start[i], finish[i], 18, 48);
			rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Failed to insert data\n");
				sqlite3_free(zErrMsg);
			}
			free(sql);
		}

	}
	sqlite3_finalize(res);



	printf("%d %d\n", cnt, cntN);

	exit(0);
}