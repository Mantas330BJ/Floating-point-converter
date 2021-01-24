#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int getFormatNumber()
{
    printf("Trumpas realus formatas 1\nIlgas realus formatas 2\nVidinis realus formatas (normalizuota forma) 3\n\n");
    int choice, scanfResult, c = '\n';
    printf("Iveskite slankaus kablelio formato numeri\n");
    while(!(scanfResult = scanf("%1d", &choice)) || ((c = getchar()) != '\n') || choice > 3 || choice < 1)
    {
        if ((!scanfResult) || (c != '\n'))
            while (getchar() != '\n');
        printf("Bloga ivestis. Formato numeris turi buti tarp 1 ir 3\n");
        printf("Iveskite slankaus kablelio formato numeri\n");
    }
    return choice;
}

int getFloatNumberAndSignBit(char number[], char integerPart[], char floatPart[])
{
    int numberLength, commaCount, goodInput = 0;
    while (!goodInput)
    {
        commaCount = 0;
        printf("Iveskite skaiciu\n");
        goodInput = 1;
        scanf("%s", number);
        numberLength = strlen(number);
        if (numberLength > 35)
        {
            printf("Per daug simboliu\n");
            goodInput = 0;
        }
        else
        {
            for (int i = 0; i < numberLength; ++i)
            {
                if ((i == 0) && ((number[i] == '-') || (number[i] == '+')))
                {
                    if (numberLength > 1)
                        continue;
                    else
                    {
                        printf("Per mazai simboliu\n");
                        goodInput = 0;
                        break;
                    }
                }
                if ((number[i] < '0') || (number[i] > '9'))
                {
                    if (number[i] == '.')
                        ++commaCount;
                    else
                    {
                        printf("Netinkami simboliai\n");
                        goodInput = 0;
                        break;
                    }
                }
            }
            if (commaCount > 1)
            {
                printf("Per daug kableliu\n");
                goodInput = 0;
            }
        }
    }

    int i, sign;
    if (number[0] == '-')
    {
        sign = 1;
        i = 1;
    }
    else if (number[0] == '+')
    {
        sign = 0;
        i = 1;
    }
    else
    {
        sign = 0;
        i = 0;
    }

    if (numberLength != 1 + i)
        while ((number[i] == '0') && (number[i + 1] == '0'))
            ++i;

    for (int j = 0; (number[i] != '.') && (number[i] != '\0'); ++i, ++j)
        integerPart[j] = number[i];
    if (number[i] == '.')
        ++i;
    for (int j = 0; number[i] != '\0'; ++i, ++j)
        floatPart[j] = number[i];
    if (floatPart[0] == '\0')
        floatPart[0] = '0';
    return sign;
}

char* multiplyStringByTwo(char numberString[], int *digits, int *bit)
{
    char output1[248] = {}, output2[248] = {};
    int opDigits, carry = 0, answerStringIndex = 0;
    char answerString[100] = {};
    *digits -= 1;
    answerStringIndex = 0;
    *bit = !(numberString[0] < '5');
    sprintf(output1, "0.%s * 2 = ", numberString);
    for (int i = strlen(numberString) - 1; i > -1; --i)
    {
        opDigits = (numberString[i] - 48) * 2 + carry;
        carry = opDigits / 10;
        answerString[answerStringIndex++] = opDigits % 10 + 48;
    }
    for (int i = answerStringIndex - 1, j = 0; i > - 1; --i, ++j)
        numberString[j] = answerString[i];
    sprintf(output2, "%d.%s\n", *bit, numberString);
    return strcat(output1, output2);
}

void printBString(int *finalStringB, int finalIndex)
{
    for (int i = 0; i < finalIndex; ++i)
        printf("%d", finalStringB[i]);
    printf("\n");
}

void printHString(int *finalStringB, int finalIndex)
{
    char hexDigits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int index;
    for (int i = 0; i < finalIndex; i += 4)
    {
        index = finalStringB[i] * 8 + finalStringB[i + 1] * 4 + finalStringB[i + 2] * 2 + finalStringB[i + 3];
        printf("%c", hexDigits[index]);
    }
    printf("\n");
}

void absoluteLessThanOne(char floatPart[], int *digits)
{
    int lastIter = 0;
    while(!lastIter)
        printf(multiplyStringByTwo(floatPart, digits, &lastIter));
}

char* convertBaseTenToTwo(char numberString[], int *index, int *digits)
{
    int numberStringLength = strlen(numberString);
    int remainder, digit, intermediate, stringIndex, maxDigits = numberStringLength, binaryDigitsIndex = 0;
    char* binaryDigits = calloc(256, sizeof(char));

    while((numberStringLength != 1) || (numberString[0] != '0'))
    {
        printf("%*s / 2  ", maxDigits, numberString);
        remainder = 0;
        stringIndex = 0;
        for (int i = 0; i < numberStringLength; ++i)
        {
            digit = numberString[i] - 48;
            digit += remainder * 10;
            intermediate = digit / 2;
            if ((stringIndex != 0) || (intermediate != 0) || (numberStringLength == 1))
                numberString[stringIndex++] = intermediate + 48;
            remainder = digit % 2;

        }
        numberString[stringIndex] = '\0';
        numberStringLength = strlen(numberString);
        printf("%*s %d\n", maxDigits, numberString, remainder);
        binaryDigits[binaryDigitsIndex++] = remainder;
        ++(*digits);
        ++(*index);
    }
    printf("\n");
    return binaryDigits;
}

void getMantissaBeginning(char integerPart[], int *finalStringB, int *finalIndex, int choice, int digits)
{
    int index = 0, index1 = 0, digits1;
    char *numberString = convertBaseTenToTwo(integerPart, &index, &digits);
    int numberStringLength = index;

    int exponents[] = {127, 1023, 16383};
    int exponent = exponents[choice - 1] + digits;
    char decimalExponent[6];
    sprintf(decimalExponent, "%d", exponent);
    printf("%d + %d = %s\n", exponents[choice - 1], digits, decimalExponent);
    char *exponentString = convertBaseTenToTwo(decimalExponent, &index1, &digits1);
    int exponentStart = *finalIndex;
    if ((integerPart[0] == '0') && (strlen(integerPart) == 1) && (digits < 1))
        finalStringB[(*finalIndex)++] = 0;

    while(index1 > 0)
        finalStringB[(*finalIndex)++] = exponentString[--index1];
    printf("Charakteristika - ");
    printBString(finalStringB + exponentStart, *finalIndex - exponentStart);

    if (choice == 3)
    {
        printf("I bitas - 1\n");
        finalStringB[(*finalIndex)++] = 1;
    }

    --index;
    int start = *finalIndex;
    while(index > 0)
        finalStringB[(*finalIndex)++] = numberString[--index];
    if (digits > 0)
    {
        printf("Mantises pradzia - ");
        printBString(finalStringB + start, numberStringLength - 1);
    }

    free(numberString);
    free(exponentString);
}

void getMantissaEnd(char floatPart[], int *finalStringB, int *finalIndex, int choice)
{
    int choices[3] = {32, 64, 80}, cycleLength, bit = 0, randomVar;
    int leftBits = choices[choice - 1], outputsIndex = 0;
    char numbers[256][64];
    char outputs[256][64] = {};



    while(*finalIndex < leftBits)
    {
        strcpy(numbers[outputsIndex], floatPart);
        strcpy(outputs[outputsIndex++], multiplyStringByTwo(floatPart, &randomVar, &bit));
        finalStringB[(*finalIndex)++] = bit;
    }

    for (int i = 0; i < outputsIndex; ++i)
    {
        for (int j = i + 1; j < outputsIndex; ++j)
        {
            if (!strcmp(numbers[i], numbers[j]))
            {
                printf("\nPeriodo pradzia\n");
                cycleLength = j - i;
                for (int k = i; k < j; ++k)
                    printf(outputs[k]);
                printf("Periodo pabaiga\n\n");
                i += (outputsIndex - i) / cycleLength * cycleLength;
                break;
            }
        }
        if (i < outputsIndex)
            printf(outputs[i]);
    }
}

int main()
{
    int choice = getFormatNumber();
    int finalStringB[128] = {}, finalIndex = 1;
    char number[64], integerPart[64] = {}, floatPart[64] = {};
    finalStringB[0] = getFloatNumberAndSignBit(number, integerPart, floatPart);

    printf("\nZenklo bitas %d\n\n", finalStringB[0]);

    int digits = -1;

    if (integerPart[0] == '0')
    {
        if ((floatPart[0] != '0') || (strlen(floatPart) != 1))
        {
            absoluteLessThanOne(floatPart, &digits);
            printf("\n");
            finalStringB[finalIndex++] = 0;
        }
        else
        {
            printf("%d", finalStringB[0]);
            char choices[][20] = {"0000000", "000000000000000", "0000000000000000000"};
            printf("%s", choices[choice-1]);
            return 0;
        }
    }

    getMantissaBeginning(integerPart, finalStringB, &finalIndex, choice, digits);
    getMantissaEnd(floatPart, finalStringB, &finalIndex, choice);
    printBString(finalStringB, finalIndex);
    printHString(finalStringB, finalIndex);

    return 0;
}
