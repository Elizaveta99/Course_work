#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wfdb/wfdb.h>
#include "sqlite3.h"

int main(int argc, char * argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *res;
	char* sql;

	rc = sqlite3_open("ECG_database.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}


	WFDB_Siginfo *s;
	int i, nsig;

	if (argc < 2) {
		fprintf(stderr, "usage: %s record\n", argv[0]);
		exit(1);
	}
	nsig = isigopen(argv[1], NULL, 0);
	if (nsig < 1) exit(2);
	s = (WFDB_Siginfo *)malloc(nsig * sizeof(WFDB_Siginfo));
	if (s == NULL) {
		fprintf(stderr, "insufficient memory\n");
		exit(3);
	}
	if (isigopen(argv[1], s, nsig) != nsig) exit(2);
	//printf("Record %s\n", argv[1]);
	printf("Starting time: %s\n", timstr(0L));
	printf("Sampling frequency: %g Hz\n", sampfreq(argv[1]));
	printf("%d signals\n", nsig);

	int signal_id = 94;                                               // менять
	for (i = 0; i < nsig; i++) 
	{
		printf("Group %d, Signal %d:\n", s[i].group, i);
		printf(" File: %s\n", s[i].fname);
		printf(" Description: %s\n", s[i].desc);
		printf(" Gain: ");
		if (s[i].gain == 0.)
			printf("uncalibrated; assume %g", WFDB_DEFGAIN);
		else printf("%g", s[i].gain);
		printf(" adu/%s\n", s[i].units ? s[i].units : "mV");
		printf(" Initial value: %d\n", s[i].initval);
		printf(" Storage format: %d\n", s[i].fmt);
		printf(" I/O: ");
		if (s[i].bsize == 0) printf("can be unbuffered\n");
		else printf("%d-byte blocks\n", s[i].bsize);
		printf(" ADC resolution: %d bits\n", s[i].adcres);
		printf(" ADC zero: %d\n", s[i].adczero);
		if (s[i].nsamp > 0L) {
			printf(" Length: %s (%ld sample intervals)\n",
				timstr(s[i].nsamp), s[i].nsamp);
			printf(" Checksum: %d\n", s[i].cksum);
		}
		else printf(" Length undefined\n");




		sql = malloc(500);
		sprintf(sql, "SELECT SignalType_id FROM SignalType WHERE signal_type = ?");
		rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

		if (rc == SQLITE_OK) {
			sqlite3_bind_text(res, 1, s[i].desc, -1, NULL); 
		}
		else {
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		}

		int step = sqlite3_step(res);

		if (step == SQLITE_ROW) {
			//printf("%s: ", sqlite3_column_text(res, 0));
			int SignalType_id = sqlite3_column_int(res, 0);
			printf("SignalType_id = %d\n", SignalType_id); // 

			free(sql);
			sql = malloc(500);
			signal_id++;
			/*char* file_name;
			file_name = malloc(20);
			sprintf(file_name, s[i].fname);
			printf("file_name = %s\n", file_name);*/
			//printf("BeatType_id = %d new_id = %d\n", BeatType_id, new_id); // 
			sprintf(sql, "INSERT INTO Signals VALUES(%d, '%s', %d, %d)", signal_id, s[i].fname, 49, SignalType_id);
			rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Failed to insert data\n");
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}

		}
		free(sql);

		//}

		sqlite3_finalize(res); // ??



	}
	exit(0);
}