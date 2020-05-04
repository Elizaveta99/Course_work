#include <stdio.h>
#include <wfdb/wfdb.h>
#include "sqlite3.h"

// in file for each class - arrays of values(y) from signals;
// common amount and each added section devides by 3 - to extract 3-seconds regions (3 * 360 = 1080 samples - values)

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

sqlite3 *db;
char *zErrMsg = 0;
int rc;
sqlite3_stmt *res;

int* data;
int record_id = 49;

static void select_region_samples(int region_type, char* file_name)
{
	FILE *file;
	char* sql;

	sql = malloc(500);
	sprintf(sql, "SELECT reg_start, reg_end FROM Regions WHERE records_id = %d AND RegionType_id = %d", record_id, region_type);
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Failed to execute statement: %s with type %d\n", sqlite3_errmsg(db), region_type);
	}
	else
	{ // there is LBBB region in this record 

		int step = sqlite3_step(res);
		if (step != SQLITE_ROW)
			fprintf(stdout, "No such records\n");

		int cntReg = 0;
		int start[2000];
		int finish[2000];

		while (step == SQLITE_ROW)
		{
			int st = sqlite3_column_int(res, 0);
			int fin = sqlite3_column_int(res, 1);

			//printf("%d %d ", st, fin);
			st /= 360;
			fin /= 360;


			//printf("%d %d\n", st, fin);


			if (fin >= 15 * 60)// last 15 minutes to test
			{
				if (st < 15 * 60)
					st = 15 * 60;

				int slice = (fin - st) / 3;
				fin = st + (slice * 3);

				if (st != fin)
				{
					start[cntReg] = st;
					finish[cntReg] = fin;
					cntReg++;
				}
			}
			step = sqlite3_step(res);
		}

		printf("cnt = %d, type = %d\n", cntReg, region_type);

		file = fopen(file_name, "a"); // append ? wb - ?
		for (int i = 0; i < cntReg; i++)
		{
			printf("start = %d finish = %d\n", start[i], finish[i]);

			//printf("test = %d file = %s\n", data[start[i] * 360], file_name);

			for (int j = start[i] * 360; j < finish[i] * 360; j++)
				fprintf(file, "%d ", data[j]);
		}
		fclose(file);
	}

	sqlite3_finalize(res);
	free(sql);
}

int main(int argc, char * argv[])
{
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

	int min30 = 30 * 60 * 360 + 1, min15 = 15 * 60 * 360;

	data = (int*)malloc(min30 * sizeof(int));
	int i_val = -1;


	for (i = 0; i < min30; i++)  // first 15 mimutes-to train,for test < 30 * 60 * 360,but start when i=15 * 60 * 360
	{
		if (getvec(v) < 0)
			break;
		//if (i >= min15)
		//{
			i_val++;
			data[i_val] = v[0];                       // 0 or 1 - where signal MLII is for each record
		//}
		//printf("%d\n", data[i_val]); 
		//fprintf(file, "%d ", data[i_val]);
	}
	printf("\n");


	select_region_samples(1, "C:\\Users\\User\\Desktop\\data_files_test\\N");
	select_region_samples(16, "C:\\Users\\User\\Desktop\\data_files_test\\LBBB");
	select_region_samples(17, "C:\\Users\\User\\Desktop\\data_files_test\\RBBB");
	select_region_samples(11, "C:\\Users\\User\\Desktop\\data_files_test\\B");
	select_region_samples(12, "C:\\Users\\User\\Desktop\\data_files_test\\T");
	select_region_samples(14, "C:\\Users\\User\\Desktop\\data_files_test\\VT");
	select_region_samples(6, "C:\\Users\\User\\Desktop\\data_files_test\\SVTA");
	select_region_samples(8, "C:\\Users\\User\\Desktop\\data_files_test\\AFIB");
	select_region_samples(7, "C:\\Users\\User\\Desktop\\data_files_test\\AFL");
	select_region_samples(10, "C:\\Users\\User\\Desktop\\data_files_test\\NOD");
	select_region_samples(13, "C:\\Users\\User\\Desktop\\data_files_test\\IVR");

	// ??
	select_region_samples(18, "C:\\Users\\User\\Desktop\\data_files_test\\V");

	sqlite3_close(db);
	exit(0);
}