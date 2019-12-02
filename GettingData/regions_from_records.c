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
		//sprintf(rhythm, annot.aux);	
		sprintf(rhythm, annstr(annot.anntyp));

		printf("annot.aux = %s!\n rhythm = %s!\n rhythm_size = %d\n beat_type = %s\n sample(time) = %d\n", annot.aux, rhythm, strlen(rhythm), annstr(annot.anntyp), annot.time);

		if (strcmp(rhythm, "L") == 0 && flagBegin == -1) // (+RBBB, +V if there are in this record)
		{
			flagBegin = 1;
	
			//sql = malloc(500);
			//sprintf(sql, "SELECT RegionType_id FROM RegionType WHERE region_type = ?");
			//rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

			//if (rc == SQLITE_OK) {
			//	//sqlite3_bind_int(res, 1, 1);
			//	
			//	sqlite3_bind_text(res, 1, rhythm, -1, NULL);
			//}
			//else {
			//	fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
			//}

			//int step = sqlite3_step(res);

			//if (step == SQLITE_ROW) {
			//	int RegionType_id = sqlite3_column_int(res, 0);
			//	printf("RegionType_id = %d\n", RegionType_id); // 
			//	regtype_id[cnt_reg] = RegionType_id;

			//}
			//free(sql);

			
			const char *attrName = NULL;
			attrName = (const char*)rhythm;

			if (attrName[2] == 'N' && strlen(attrName) == 3)
				regtype_id[cnt_reg] = 1;
			else if (attrName[2] == 'N' && strlen(attrName) == 5)
				regtype_id[cnt_reg] = 10;
			else if (attrName[2] == 'S' && strlen(attrName) == 5)
				regtype_id[cnt_reg] = 2;
			else if (attrName[2] == 'S' && strlen(attrName) == 6)
				regtype_id[cnt_reg] = 6;
			else if (attrName[2] == 'B' && strlen(attrName) == 3)
				regtype_id[cnt_reg] = 11;
			else if (attrName[2] == 'B' && strlen(attrName) == 5)
				regtype_id[cnt_reg] = 3;
			else if (attrName[2] == 'P' && strlen(attrName) == 3)
				regtype_id[cnt_reg] = 9;
			else if (attrName[2] == 'P' && strlen(attrName) == 6)
				regtype_id[cnt_reg] = 4;
			else if (attrName[2] == 'A' && strlen(attrName) == 4)
				regtype_id[cnt_reg] = 5;
			else if (attrName[2] == 'A' && strlen(attrName) == 5)
				regtype_id[cnt_reg] = 7;
			else if (attrName[2] == 'A' && strlen(attrName) == 6)
				regtype_id[cnt_reg] = 8;
			else if (attrName[2] == 'T' && strlen(attrName) == 3)
				regtype_id[cnt_reg] = 12;
			else if (attrName[2] == 'I' && strlen(attrName) == 5)
				regtype_id[cnt_reg] = 13;
			else if (attrName[2] == 'V' && strlen(attrName) == 4)
				regtype_id[cnt_reg] = 14;
			else if (attrName[2] == 'V' && strlen(attrName) == 5)
				regtype_id[cnt_reg] = 15;
			else regtype_id[cnt_reg] = -1; // annot.aux = null

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

			printf("annot.aux = %s!\n rhythm = %s!\n rhythm_size = %d\n beat_type = %s\n sample(time) = %d\n", annot.aux, rhythm, strlen(rhythm), annstr(annot.anntyp), annot.time);

		}
		else
			if (strcmp(rhythm, "L") == 0 && flagBegin != -1)
			{
				finish_prev = annot.time;
			}
			else
			if (strcmp(rhythm, "L") != 0) 
			{
				//finish_prev = annot.time;
				flagBegin = -1;	// previous L-region efinished, the new one is beginning
			}

		//sql = malloc(500);
		//sprintf(sql, "SELECT BeatType_id FROM BeatType WHERE beat_type = ?");
		//rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

		//if (rc == SQLITE_OK) {
		//	//sqlite3_bind_int(res, 1, beat);
		//	sqlite3_bind_text(res, 1, beat, -1, NULL); // ?? parameters
		//}
		//else {
		//	fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		//}

		//int step = sqlite3_step(res);

		//if (step == SQLITE_ROW) {
		//	//printf("%s: ", sqlite3_column_text(res, 0));
		//	int BeatType_id = sqlite3_column_int(res, 0);
		//	//printf("BeatType_id = %d\n", BeatType_id); // 

		//	free(sql);
		//	sql = malloc(500);
		//	new_id++;
		//	//printf("BeatType_id = %d new_id = %d\n", BeatType_id, new_id); // 
		//	sprintf(sql, "INSERT INTO Beats VALUES(%d, %d, %d, %d)", new_id, annot.time, BeatType_id, 41);
		//	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
		//	if (rc != SQLITE_OK) {
		//		fprintf(stderr, "Failed to insert data\n");
		//		sqlite3_free(zErrMsg);
		//	}

		//}
		//free(sql);

		//sqlite3_finalize(res); // ??

		if (strcmp(annstr(annot.anntyp), "N") != 0)
		{
			cntN++;
		}
		free(rhythm);
	}

	finish[cnt_reg - 1] = finish_prev;  // finish time for last region

	printf("\nreg_amount = %d\n", cnt_reg);

	int new_id = 1725;                                    // с этими данными ещё не вставляла!! - запись 112

	for (int i = 0; i < cnt_reg; i++)
	{
		printf("type = %d\n start = %d\n finish = %d\n", regtype_id[i], start[i], finish[i]);

			sql = malloc(500);
			new_id++;
			//printf("BeatType_id = %d new_id = %d\n", BeatType_id, new_id); // 
			sprintf(sql, "INSERT INTO Regions VALUES(%d, %d, %d, %d, %d)", new_id, start[i], finish[i], regtype_id[i], 49);
			rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Failed to insert data\n");
				sqlite3_free(zErrMsg);
			}
			free(sql);

	}
	sqlite3_finalize(res);

	

	printf("%d %d\n", cnt, cntN);

	exit(0);
}