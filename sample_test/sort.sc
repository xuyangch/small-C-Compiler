int a[10] = {-1,134,124,64,908,1324,5243,2,0,-341};
int sort(int len)
{
    int i=0;
    int j;
    int t;
    for(i=0;i<len;++i)
    {
        for(j=0;j<len-i-1;++j)
        {
            if(a[j]>a[j+1])
            {
                t=a[j];
                a[j]=a[j+1];
                a[j+1]=t;
            }
        }
    }
    return 0;
}
int main()
{
    int i=0;
    sort(10);
    for(i=0;i<10;++i)
    {
        write(a[i]);
    }
    return 0;
}