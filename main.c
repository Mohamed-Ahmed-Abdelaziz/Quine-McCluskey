#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define MAX 200

int colAdder(int row, int col, int arr[row][col], int desiredCol);

int colOnesAdder(int row, int col, int arr[row][col], int desiredCol);

int rowIndex(int row, int col, int arr[row][col], int flagedCol);

int main(void) {
    //receiving the inputs from the user

    int noVariables, noMinterms = 0, valuesMinterms, temp1, temp2;
    printf("Number of variables: ");
    scanf("%d", &temp1);
    if(temp1 <= 0){
        printf("Error\n");
        exit(0);
    }
    else
        noVariables = temp1;
    printf("Number of minterms: ");
    scanf("%d", &temp2);
    if((temp2 <= pow(2, noVariables)) && temp2 > 0)
        noMinterms = temp2;
    else {
        printf("Error\n");
        exit(0);
    }
    printf("Values of minterms: ");
    int primeImplicant[MAX][noVariables]; //matrix of prime implicants
    int rPrime = 0, flag = 0;
    memset(primeImplicant, 0, sizeof(primeImplicant[0][0]) * MAX * noVariables);
    int stage1[noMinterms][noVariables + 3];
    memset(stage1, 0, sizeof(stage1[0][0])* noMinterms *(noVariables + 3));
    for(int i = 0; i < noMinterms; i++) {
        int temp3 = 0;
        scanf("%d", &temp3);
        if ((temp3 < pow(2, noVariables)) && temp3 >= 0)
            stage1[i][noVariables + 1] = temp3;
        else {
            printf("Error\n");
            i--;
        }
    }
    //converting minterms from decimal to binary

    for(int i = 0; i < noMinterms; i++){
        int c = noVariables - 1;
        for(int j = 0; j < noVariables; j++){
            int m;
            m = stage1[i][noVariables + 1] >> c;
            if (m & 1){
                stage1[i][j] = 1;
            }
            else
                stage1[i][j] = 0;
            c--;
        }
    }
    //counting ones in binary representation of minterms
    for(int i = 0; i < noMinterms; i++){
        int counter = 0;
        for(int j = 0; j < noVariables; j++) {
            if (stage1[i][j] == 1)
                counter++;
        }
        stage1[i][noVariables] = counter;
    }
    //sorting stage1 with respect to no of ones in binary representation of minterms

    for(int i = 0; i < noMinterms - 1; i++){
        for(int j = i + 1; j < noMinterms; j++){
            if(stage1[i][noVariables] > stage1[j][noVariables]){
                for(int k = 0; k < noVariables + 3; k++){
                    stage1[i][k] ^= stage1[j][k];
                    stage1[j][k] ^= stage1[i][k];
                    stage1[i][k] ^= stage1[j][k];
                }
            }
        }
    }
//Comparison of stage1 into stage2

int row = MAX;
    int stage2[row][noVariables + 3];
    memset(stage2, 0, sizeof(stage2[0][0])* row *(noVariables + 3));
    int r2 = 0;
    for(int i = 0; i < noMinterms - 1; i++) {
        int j = i + 1;
        //comparison according to no of ones and diff of binary representation
        while (j < noMinterms) {
            if (abs(stage1[i][noVariables] - stage1[j][noVariables]) == 1) {
                int noDiff = 0;
                for (int m = 0; m < noVariables; m++) {
                    if (stage1[i][m] != stage1[j][m]) {
                        noDiff++;
                    }
                }
                if (noDiff == 1) {
                    for (int n = 0; n < noVariables ; n++) {
                    if(stage1[i][n] == stage1[j][n])
                        stage2[r2][n] = stage1[i][n];
                    else
                        stage2[r2][n] = 2;
                    }
                    //marking paired minterms in stage1
                    stage1[i][noVariables + 2] = 1;
                    stage1[j][noVariables + 2] = 1;
                    r2++;
                }
            }
            j++;
        }
    }

    //prime implicant of stage1

    for(int i = 0; i < noMinterms; i++){
        if(stage1[i][noVariables + 2] == 0){
            for(int j = 0; j < noVariables; j++){
                primeImplicant[rPrime][j] = stage1[i][j];
            }
            rPrime++;
        }
    }
    if(rPrime != noMinterms)
        flag = 1;

//    for(int i = 0; i < noMinterms; i++){
//        for(int j = 0; j < noVariables + 3; j++){
//            printf("%d  ",stage1[i][j]);
//        }
//        printf("\n");
//    }

//counting no of ones and dashes"2" in stage2

    for(int i = 0; i < r2; i++){
        for(int j = 0; j < noVariables; j++){
            if(stage2[i][j] == 1)
                stage2[i][noVariables]++;
            else if(stage2[i][j] == 2)
                stage2[i][noVariables +1]++;
        }
    }
//    printf("\n\n");
//    for (int i = 0; i < r2; ++i) {
//        for (int j = 0; j < noVariables +3; ++j) {
//            printf("%d  ", stage2[i][j]);
//        }
//        printf("\n");
//    }
    int stage3[row][noVariables + 3];
    int r3;
    //rest of comparison between stage2 and stage3 and vice verca to reach the final stage
    for(int counter = 1; (counter <= noVariables - 2) && (flag == 1);counter++ ){
        flag = 0;
        if(counter % 2 == 1){
            memset(stage3, 0, sizeof(stage3[0][0])* row *noVariables);
            r3 = 0;
            for(int i = 0; i < r2 - 1 ; i++) {
                int j = i + 1;
                while (j < r2) {
                    if ((abs(stage2[i][noVariables] - stage2[j][noVariables]) == 1) && (stage2[i][noVariables + 1] == stage2[j][noVariables + 1])){
                        int noDiff = 0;
                        for (int m = 0; m < noVariables; m++) {
                            if (stage2[i][m] != stage2[j][m]) {
                                noDiff++;
                            }
                        }
                        if (noDiff == 1) {
                            for (int n = 0; n < noVariables ; n++) {
                                if(stage2[i][n] == stage2[j][n])
                                    stage3[r3][n] = stage2[i][n];
                                else
                                    stage3[r3][n] = 2;
                            }
                            stage2[i][noVariables + 2] = 1;
                            stage2[j][noVariables + 2] = 1;
                            r3++;
                        }
                    }
                    j++;
                }

            }
            for(int i = 0; i < r3; i++){
                for(int j = 0; j < noVariables; j++){
                    if(stage3[i][j] == 1)
                        stage3[i][noVariables]++;
                    else if(stage3[i][j] == 2)
                        stage3[i][noVariables +1]++;
                }
            }
            for(int i = 0; i < r2; i++){
                if(stage2[i][noVariables + 2] == 0){
                    for(int j = 0; j < noVariables; j++){
                        primeImplicant[rPrime][j] = stage2[i][j];
                    }
                    rPrime++;
                }
                else
                    flag = 1;
            }

        }
        else {
            memset(stage2, 0, sizeof(stage2[0][0])* row *(noVariables + 3));
            r2 = 0;
            for(int i = 0; i < r3 - 1 ; i++) {
                int j = i + 1;
                while (j < r3) {
                    if ((abs(stage3[i][noVariables] - stage3[j][noVariables]) == 1) && (stage3[i][noVariables + 1] == stage3[j][noVariables + 1])){
                        int noDiff = 0;
                        for (int m = 0; m < noVariables; m++) {
                            if (stage3[i][m] != stage3[j][m]) {
                                noDiff++;
                            }
                        }
                        if (noDiff == 1) {
                            for (int n = 0; n < noVariables ; n++) {
                                if(stage3[i][n] == stage3[j][n])
                                    stage2[r2][n] = stage3[i][n];
                                else
                                    stage2[r2][n] = 2;
                            }
                            stage3[i][noVariables + 2] = 1;
                            stage3[j][noVariables + 2] = 1;
                            r2++;
                        }
                    }
                    j++;
                }

            }
            for(int i = 0; i < r2; i++) {
                for (int j = 0; j < noVariables; j++) {
                    if (stage2[i][j] == 1)
                        stage2[i][noVariables]++;
                    else if (stage2[i][j] == 2)
                        stage2[i][noVariables + 1]++;
                }
            }
            for(int i = 0; i < r3; i++){
                if(stage3[i][noVariables + 2] == 0){
                    for(int j = 0; j < noVariables; j++){
                        primeImplicant[rPrime][j] = stage3[i][j];
                    }
                    rPrime++;
                }
                else
                    flag = 1;
            }
        }
    }
    int primeImp_unrepeated[rPrime][noVariables]; //creating prime implicants matrix without repititon
    memset(primeImp_unrepeated, 0, sizeof(primeImp_unrepeated[0][0])* rPrime * noVariables);
    int rPrime_unrepeated = 0;
    for (int i = 0; i < rPrime; i++) {
        int flag1 = 0;
        for(int j = i + 1; j < rPrime; j++){
            int flag2 = 0;
            for(int k = 0; k < noVariables; k++){
                if(primeImplicant[i][k] != primeImplicant[j][k]){
                    flag2 = 1;
                }
            }
            if(flag2 == 0)
                flag1 = 1;
        }
        if(flag1 == 0) {
            for (int k = 0; k < noVariables; k++) {
                primeImp_unrepeated[rPrime_unrepeated][k] = primeImplicant[i][k];
            }
            rPrime_unrepeated++;
        }
    }
    printf("Number of prime implicants: %d\n", rPrime_unrepeated);
    for(int i = 0; i < rPrime_unrepeated; i++){
        for(int j = 0; j < noVariables; j++){
            printf("%d  ", primeImp_unrepeated[i][j]);
        }
        printf("\n");
    }
    // making and printing literals of prime implicants.
    int indexx = 0;
    char literals[rPrime_unrepeated][noVariables * 3];
    for(int i = 0; i < rPrime_unrepeated; ++i){
        for(int j = 0; j < noVariables; ++j){
                if(primeImp_unrepeated[i][j] == 0){
                    literals[i][indexx++] = 65 + j;
                    literals[i][indexx++] = '`';
                }
                else if(primeImp_unrepeated[i][j] == 1){
                    literals[i][indexx++] = 65 + j;
                }
        }
        literals[i][indexx] = '\0';
        indexx = 0;
    }

    printf("prime implicants: ");
    for(int i = 0; i < rPrime_unrepeated; ++i){
        if(i == rPrime_unrepeated - 1){
        printf("%s ", literals[i]);
        }else printf("%s, ", literals[i]);
    }
    //aewfefewgw
    int temp_a = 0;
    int temp_power_dashes = 0;
    int index2 = 0;
    int tempor = 0;
    int minterms_included[rPrime_unrepeated][25];
    int binary_repr[50][noVariables];
    int no_of_dashes = 0;
    for(int i = 0; i < rPrime_unrepeated; ++i){
        no_of_dashes = 0;
        for(int j = 0; j < noVariables; ++j){
            if(primeImp_unrepeated[i][j] == 2) ++no_of_dashes;
        }
        if(no_of_dashes == 0){
            for(int j = 0; j < noVariables; ++j) tempor += pow(2, noVariables-1-j) * primeImp_unrepeated[i][j];
            minterms_included[i][0] = tempor;
            minterms_included[i][1] = -1;
            tempor = 0;
        }
        else if(no_of_dashes == 1){
            for(int j = 0; j < noVariables; ++j){
                if(primeImp_unrepeated[i][j] == 2) index2 = j;
            }
            for(int j = 0; j < noVariables; ++j){
                if(j == index2){tempor += 0;}
                else tempor += pow(2, noVariables-1-j) * primeImp_unrepeated[i][j];
            }
            minterms_included[i][0] = tempor;
            tempor = 0;

            for(int j = 0; j < noVariables; ++j){
                if(j == index2){tempor += pow(2, noVariables-1-index2);}
                else tempor += pow(2, noVariables-1-j) * primeImp_unrepeated[i][j];
            }
            minterms_included[i][1] = tempor;
            minterms_included[i][2] = -1;
            tempor = 0;
            index2 = 0;
        }
        else{
            int power_dashes = pow(no_of_dashes,2);
            int a1 = 1, b1 = 0, c1;
            temp_power_dashes = power_dashes;

            for(int j = 0; j < noVariables; ++j){
                if(primeImp_unrepeated[i][j] == 0){
                    for(int k = 0; k < pow(no_of_dashes,2); ++k){
                        binary_repr[k][j] = 0;
                    }
                }
                else if(primeImp_unrepeated[i][j] == 1){
                    for(int k = 0; k < pow(no_of_dashes,2); ++k){
                        binary_repr[k][j] = 1;
                    }
                }
                else if(primeImp_unrepeated[i][j] == 2){
                    power_dashes /= 2;
                    for(int k = 0; k < pow(no_of_dashes,2); ++k){
                        if(k % power_dashes == 0){
                            c1 = a1;
                            a1 = b1;
                            b1 = c1;
                        }
                        binary_repr[k][j] = a1;
                    }
                }
            }
            for(int a = 0; a < temp_power_dashes; ++a){
                for(int b = 0; b < noVariables; ++b){
                    tempor += pow(2, noVariables-1-b) * binary_repr[a][b];
                }
                minterms_included[i][a] = tempor;
                tempor = 0;
                temp_a = a;
            }
            minterms_included[i][temp_a + 1] = -1;
        }
    }

    int primeinplicant_table[rPrime_unrepeated + 5][noMinterms + 5];
    memset(primeinplicant_table, 0, sizeof primeinplicant_table);
    for(int i = 0; i < noMinterms; ++i){
        primeinplicant_table[rPrime_unrepeated][i] = stage1[i][noVariables + 1];
    }
    for(int i = 0; i < rPrime_unrepeated; ++i){
        for(int j = 0; j < 100; ++j){
            if(minterms_included[i][j] == -1) break;
            for(int k = 0; k < noMinterms; ++k){
                if(minterms_included[i][j] == primeinplicant_table[rPrime_unrepeated][k]){
                    primeinplicant_table[i][k] = 1;
                }
            }
        }
    }

    // Printing prime implicant table.
    printf("\n\nPrime implicants table:\n\nMinterms\t");
    for(int i = 0; i < noMinterms; ++i){
        printf("%d\t", primeinplicant_table[rPrime_unrepeated][i]);

    }
    for(int i = 0; i < rPrime_unrepeated; ++i){
        printf("\n\n%s\t\t", literals[i]);
        for(int j = 0; j < noMinterms; ++j){
        printf("%d\t", primeinplicant_table[i][j]);
        }
    }

    //----------------------------------------------
    // counting the cost of prime implicants and storing it in primeinplicant_table[i][noMinterms+1]
    for(int i = 0; i < rPrime_unrepeated; i++){
        int costCounter = 0;
        for(int j = 0; j < noVariables; j++){
            if(primeImp_unrepeated[i][j] != 2)
            {
                costCounter ++;
            }
        if (costCounter != 1)
            primeinplicant_table[i][noMinterms+1] = costCounter + 1;
        else
            primeinplicant_table[i][noMinterms+1] = costCounter;
        }
    }

    int changeOccured = 0;
    for (int f = 0; f< noMinterms; f++)
    {
        //----------------------------------------------
        // Finding essential prime implicants
        for (int i = 0; i < rPrime_unrepeated; i ++)
        {
            for (int j = 0; j < noMinterms; j++)
            {
                if (colAdder(rPrime_unrepeated  ,noMinterms + 5,primeinplicant_table,j) == 1)
                {
                    int essRow = rowIndex(rPrime_unrepeated ,noMinterms + 5,primeinplicant_table,j);
                    if (f == 0)
                    {
                        primeinplicant_table[essRow][noMinterms] = 1 ;
                    }else{
                        primeinplicant_table[essRow][noMinterms] = 2 ;
                    }
                    for (int k = 0; k<noMinterms; k++)
                    {
                        if (primeinplicant_table[essRow][k]==1)
                        {
                            for (int l = 0; l<rPrime_unrepeated; l++)
                            {
                                primeinplicant_table[l][k] = 0;
                            }
                        }
                    }
                }
            }
        }
        //----------------------------------------------
        // counting number of ones in each row and storing it in primeinplicant_table[i][noMinterms+2]
        // counting number of ones in each col and storing it in primeinplicant_table[rPrime_unrepeated+1][j]
        for (int i = 0; i<rPrime_unrepeated; i++)
        {
            int sum = 0, j ;
            for (j = 0; j<noMinterms; j++)
            {
                sum += primeinplicant_table[i][j];
            }
            primeinplicant_table[i][noMinterms+2] = sum;
        }
        for (int j = 0; j<noMinterms; j++)
        {
            primeinplicant_table[rPrime_unrepeated+1][j] = colAdder(rPrime_unrepeated, noMinterms, primeinplicant_table, j);
        }
        //----------------------------------------------
        // Row Dominance
        for (int i = 0; i<rPrime_unrepeated; i++)
        {
            for (int j = 0; j<rPrime_unrepeated; j++)
            {
                if(primeinplicant_table[i][noMinterms+2]>=primeinplicant_table[j][noMinterms+2] && i!=j && primeinplicant_table[i][noMinterms+1]<=primeinplicant_table[j][noMinterms+1])
                {
                    int flagg = 0;
                    for (int k = 0; k<noMinterms; k++)
                    {
                        if(primeinplicant_table[i][k] >= primeinplicant_table[j][k])
                        {
                            flagg = 1;
                        } else {
                        flagg = 0;
                        break;
                        }
                    }
                    if (flagg == 1)
                    {
                        changeOccured = 1;
                        for (int l = 0; l<noMinterms; l++)
                        {
                            primeinplicant_table[j][l] = 0;
                        }
                        primeinplicant_table[j][noMinterms+2] = 0;
                    }
                }
            }
        }
        if (changeOccured==1)
            continue;
        //----------------------------------------------
        // Col Dominance
        for (int i = 0; i<noMinterms; i++)
        {
            for (int j = 0; j<noMinterms; j++)
            {
                if(primeinplicant_table[rPrime_unrepeated+1][i]>=primeinplicant_table[rPrime_unrepeated+1][j] && i!=j )
                {
                    int flagg = 0;
                    for (int k = 0; k<rPrime_unrepeated; k++)
                    {
                        if(primeinplicant_table[k][i] >= primeinplicant_table[k][j])
                        {
                            flagg = 1;
                        } else {
                        flagg = 0;
                        break;
                        }
                    }
                    if (flagg == 1)
                    {
                        for (int l = 0; l<rPrime_unrepeated; l++)
                        {
                            primeinplicant_table[l][i] = 0;
                        }
                        primeinplicant_table[rPrime_unrepeated+1][i] = 0;
                    }
                }
            }
        }
        if (changeOccured==1)
            continue;
    }

    //----------------------------------------------
    //printing essential prime implicants
    int noEssPr = colOnesAdder(rPrime_unrepeated +5, noMinterms + 5, primeinplicant_table, noMinterms);
    printf("\n\nNumber of Essential Prime Implicants: %d", noEssPr);
    printf("\n\nEssential Prime Implicants:");
    for (int i = 0; i<rPrime_unrepeated; i++)
    {
        if (primeinplicant_table[i][noMinterms] == 1 && i+1 !=rPrime_unrepeated)
        {
            printf(" %s,", literals[i]);
        }else if (primeinplicant_table[i][noMinterms] == 1)
        {
         printf(" %s", literals[i]);
        }
    }

    //----------------------------------------------
    // printing Result
    printf("\n\nResult: f =");

    if(noEssPr != 0)
    {
        for (int i = 0; i<rPrime_unrepeated; i++)
        {
            if (primeinplicant_table[i][noMinterms] != 0 )
            {
                if (i!=rPrime_unrepeated-1)
                    printf(" %s +", literals[i]);
                else
                    printf(" %s ", literals[i]);
            }
        }
    }else{
        for (int i = 0; i<rPrime_unrepeated; i++)
        {
            if (i!=rPrime_unrepeated-1)
                printf(" %s +", literals[i]);
            else
                printf(" %s ", literals[i]);
        }
    }
    printf("\n");


    return 0;
}
int colAdder(int row, int col, int arr[row][col], int desiredCol)
{
    int sum = 0, i ;
    for (i = 0; i<row; i++)
    {
        sum += arr[i][desiredCol];
    }
    return sum ;
}
int colOnesAdder(int row, int col, int arr[row][col], int desiredCol)
{
    int sum = 0, i ;
    for (i = 0; i<row; i++)
    {
        if (arr[i][desiredCol] == 1)
        {
            sum += arr[i][desiredCol];
        }
    }
    return sum ;
}
int rowIndex(int row, int col, int arr[row][col], int flagedCol)
{
    int reqIndex ;
    for (int i = 0; i<row; i++)
    {
        if (arr[i][flagedCol]==1)
            reqIndex = i;
    }
    return reqIndex;
}
/*void rowColRemove(int row, int col, int arr[row][col], int desiredRow)
{
    for (int j = 0; j<col; j++)
        {
            if (arr[desiredRow][j]==1)
            {
                for (int i = 0; i<row; i++)
                {
                    arr[i][j] = 0;
                }
            }
        }
}*/
/*int rowAdder(int row, int col, int arr[row][col], int desiredRow)
{
    int sum = 0, i ;
    for (i = 0; i<col; i++)
    {
        sum += arr[desiredRow][i];
    }
    return sum ;
}*/
