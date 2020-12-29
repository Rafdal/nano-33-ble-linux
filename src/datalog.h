#include <SD.h>

class datalog
{
private:
    String name;
    size_t file_idx=0;

public:
    datalog();
    ~datalog();

    
};

datalog::datalog()
{
}

datalog::~datalog()
{
}

datalog :: log()
{
    for (; file_idx < 1024; file_idx++)    
    {
        String filename = name + String(file_idx) + ".csv";
        if (SD.exists(filename)) {;}
        else {break;}
    }

    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile) 
    {
        dataFile.println("x,\ty,\tz");
        for (uint16_t i = 0; i < n_cols ; i++)
        {
            dataFile.print(i);
            Serial.print(i);
            if (i < n_cols-1)
            {
                dataFile.print(',');
                Serial.print(',');
            }            
        }
        dataFile.println();
        Serial.println();
    }
    else
    {
        Serial.println(F("Error abriendo el archivo"));
    }
    dataFile.close();
}
