#include <stdio.h>

// This Function Counts the Pieces// 

int count9(int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9)
{
    return a1+a2+a3+a4+a5+a6+a7+a8+a9;
}

// This function gets the coordinates // 

void setCell(int a,int b,int val,
    int *c11,int *c12,int *c13,int *c21,int *c22,int *c23,int *c31,int *c32,int *c33)
{
    if(a==1&&b==1)*c11=val;
    else if(a==1&&b==2)*c12=val;
    else if(a==1&&b==3)*c13=val;
    else if(a==2&&b==1)*c21=val;
    else if(a==2&&b==2)*c22=val;
    else if(a==2&&b==3)*c23=val;
    else if(a==3&&b==1)*c31=val;
    else if(a==3&&b==2)*c32=val;
    else if(a==3&&b==3)*c33=val;
}

int getCell(int a,int b,
    int c11,int c12,int c13,int c21,int c22,int c23,int c31,int c32,int c33)
{
    int v=0;
    if(a==1&&b==1)v=c11;
    else if(a==1&&b==2)v=c12;
    else if(a==1&&b==3)v=c13;
    else if(a==2&&b==1)v=c21;
    else if(a==2&&b==2)v=c22;
    else if(a==2&&b==3)v=c23;
    else if(a==3&&b==1)v=c31;
    else if(a==3&&b==2)v=c32;
    else if(a==3&&b==3)v=c33;
    return value;
}

/* Expand */
void Expand(int a,int b,int *go,
    int *R11,int *R12,int *R13,int *R21,int *R22,int *R23,int *R31,int *R32,int *R33,
    int *B11,int *B12,int *B13,int *B21,int *B22,int *B23,int *B31,int *B32,int *B33,
    int *S11,int *S12,int *S13,int *S21,int *S22,int *S23,int *S31,int *S32,int *S33,
    int *T11,int *T12,int *T13,int *T21,int *T22,int *T23,int *T31,int *T32,int *T33)
{
    int found;

    Remove(a,b,*go,
        R11,R12,R13,R21,R22,R23,R31,R32,R33,
        B11,B12,B13,B21,B22,B23,B31,B32,B33,
        S11,S12,S13,S21,S22,S23,S31,S32,S33,
        T11,T12,T13,T21,T22,T23,T31,T32,T33);

    if(*go==1 && a-1>=1)
        Replace(a-1,b,go,&found,
            R11,R12,R13,R21,R22,R23,R31,R32,R33,
            B11,B12,B13,B21,B22,B23,B31,B32,B33,
            S11,S12,S13,S21,S22,S23,S31,S32,S33,
            T11,T12,T13,T21,T22,T23,T31,T32,T33);

    if(*go==0 && a+1<=3)
        Replace(a+1,b,go,&found,
            R11,R12,R13,R21,R22,R23,R31,R32,R33,
            B11,B12,B13,B21,B22,B23,B31,B32,B33,
            S11,S12,S13,S21,S22,S23,S31,S32,S33,
            T11,T12,T13,T21,T22,T23,T31,T32,T33);

    if(b-1>=1)
        Replace(a,b-1,go,&found,
            R11,R12,R13,R21,R22,R23,R31,R32,R33,
            B11,B12,B13,B21,B22,B23,B31,B32,B33,
            S11,S12,S13,S21,S22,S23,S31,S32,S33,
            T11,T12,T13,T21,T22,T23,T31,T32,T33);

    if(b+1<=3)
        Replace(a,b+1,go,&found,
            R11,R12,R13,R21,R22,R23,R31,R32,R33,
            B11,B12,B13,B21,B22,B23,B31,B32,B33,
            S11,S12,S13,S21,S22,S23,S31,S32,S33,
            T11,T12,T13,T21,T22,T23,T31,T32,T33);
}

/* Update */
void Update(int a,int b,int *go,int *good,
    int *R11,int *R12,int *R13,int *R21,int *R22,int *R23,int *R31,int *R32,int *R33,
    int *B11,int *B12,int *B13,int *B21,int *B22,int *B23,int *B31,int *B32,int *B33,
    int *S11,int *S12,int *S13,int *S21,int *S22,int *S23,int *S31,int *S32,int *S33,
    int *T11,int *T12,int *T13,int *T21,int *T22,int *T23,int *T31,int *T32,int *T33)
{
    if(getCell(a,b,*S11,*S12,*S13,*S21,*S22,*S23,*S31,*S32,*S33)==0)
    {
        setCell(a,b,1,S11,S12,S13,S21,S22,S23,S31,S32,S33);
        *good = 1;
    }
    else if(getCell(a,b,*T11,*T12,*T13,*T21,*T22,*T23,*T31,*T32,*T33)==0)
    {
        setCell(a,b,1,T11,T12,T13,T21,T22,T23,T31,T32,T33);
        Expand(a,b,go,
            R11,R12,R13,R21,R22,R23,R31,R32,R33,
            B11,B12,B13,B21,B22,B23,B31,B32,B33,
            S11,S12,S13,S21,S22,S23,S31,S32,S33,
            T11,T12,T13,T21,T22,T23,T31,T32,T33);
        *good = 1;
    }
    else
    {
        *good = 0;
    }
}

