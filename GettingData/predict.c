#include <stdio.h>
#include <wfdb/wfdb.h>
#include "sqlite3.h"

// in file for each class - arrays of values(y) from signals;
// common amount and each added section devides by 3 - to extract 3-seconds regions (3 * 360 = 1080 samples - values)

int* data;
FILE *file;

int main(int argc, char * argv[])
{

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

	int min30 = 30 * 60 * 360, min15 = 15 * 60 * 360;

	data = (int*)malloc(min30 * sizeof(int));
	int i_val = -1;

	for (i = 0; i < min30; i++)  // first 15 mimutes-to train,for test < 30 * 60 * 360,but start when i=15 * 60 * 360
	{
		if (getvec(v) < 0)
			break;
		i_val++;
		data[i_val] = v[0];                       // 0 or 1 - where signal MLII is for each record
	//}
	//printf("%d\n", data[i_val]); 
	//fprintf(file, "%d ", data[i_val]);
	}
	printf("\n");

	file = fopen("C:\\Users\\User\\Desktop\\data_files_predict_4\\118", "a");
	for (int j = 0; j < min30; j++)
		fprintf(file, "%d ", data[j]);

	fclose(file);
	exit(0);
}