#include <SPI.h>
#include <SD.h>

typedef struct datalog_type
{
    float x[SAMPLING];
	float y[SAMPLING];
	float z[SAMPLING];
}datalog_type_t;


class Datalog
{
private:
    size_t file_idx=0;

public:
    Datalog(){}
    ~Datalog(){}

    void log(datalog_type_t data, String name);
};


void Datalog :: log(datalog_type_t data, String name)
{
    String filename;
    for (; file_idx < 1024; file_idx++)    
    {
        filename = name + String(file_idx) + ".csv";
        if (SD.exists(filename)) {;}
        else {break;}
    }

    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile) 
    {
        Serial.println(F("Archivo abierto correctamente"));
        dataFile.println("timestamp,x,y,z");
        for (uint16_t i = 0; i < SAMPLING ; i++)
        {
            dataFile.print(i);
            dataFile.print(",");
            dataFile.print(data.x[i]);
            dataFile.print(",");
            dataFile.print(data.y[i]);
            dataFile.print(",");
            dataFile.print(data.z[i]);
            dataFile.println();
        }
    }
    else
    {
        Serial.println(F("Error abriendo el archivo"));
    }
    dataFile.close();
}
