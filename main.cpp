#include "CppSQLite3.h"
#include <iostream>
#include <sys/timeb.h>
#include<string>
int main(int argc, char *argv[])
{
    const size_t INSERT_CNT = 100*10000;
    CppSQLite3DB dbHandle;

    try
    {
        dbHandle.open("./faceInfo.db");
        dbHandle.execDML("PRAGMA synchronous = OFF; ");
        dbHandle.execDML("PRAGMA journal_mode=WAL; ");
        std::cout<<"open faceInfo.db success!"<<std::endl;


        std::string strSql = "SELECT camid, featureData, kfktimestamp, trackid, statecode, aligndata, sectionaldrawdata, float FROM snapface;";
        struct timeb tb;
        ftime(&tb);
        std::cout<<"start timestamp=" << tb.time<<std::endl;

        //开始事务
        dbHandle.execDML("begin transaction;");
        CppSQLite3Query q = dbHandle.execQuery(strSql.c_str());
        std::string camid, timestamp, aligndata, drawdata;
        int trackid, statecode, blobLen;
        float fTest;
        int i =0;
        const unsigned char * blobData;
        while (!q.eof())
        {
            camid = q.getStringField(0);
            blobData = q.getBlobField(1, blobLen);
            timestamp = (q.getStringField(2));
            trackid = q.getIntField(3);
            statecode = q.getIntField(4);
            aligndata = q.getStringField(5);
            drawdata = q.getStringField(6);
            fTest = q.getFloatField(7);
            q.nextRow();
            if(i++ == INSERT_CNT-2)
            {
                for(int j=0; j<blobLen; j++)
                {
                    printf("blobData[%d]=%d\n", j, blobData[j]);
                }
            }
        }
        dbHandle.execDML("commit transaction;");

        std::cout<<"read features cnt = " << i << " finish!"<<std::endl;
        struct timeb tend;
        ftime(&tend);
        std::cout<<"start timestamp=" << tend.time<<std::endl;
        std::cout<<"read consume time=" << (tend.time*1000+tend.millitm - tb.time*1000-tb.millitm)/1000.0f << "S" <<std::endl;

        std::cout << "camid=" << camid << std::endl;
        std::cout << "timestamp=" << timestamp << std::endl;
        std::cout << "aligndata=" << aligndata << std::endl;
        std::cout << "drawdata=" << drawdata << std::endl;
        std::cout << "float=" << fTest << std::endl;
    }
    catch (CppSQLite3Exception & e)
    {
        printf("%s", e.errorMessage());
        dbHandle.execDML("rollback transaction;");
    }
    catch (...)
    {
        printf("ERROR_LOCALFEATURE_SQLITE3_INSERT");
        dbHandle.execDML("rollback transaction;");
    }
}
