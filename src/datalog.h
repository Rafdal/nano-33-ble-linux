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
    String name;
    size_t file_idx=0;

public:
    Datalog(String _name);
    ~Datalog();

    void log(datalog_type_t data);
};

Datalog :: Datalog(String _name)
{
    name = _name;
}

Datalog :: ~Datalog(){}


void Datalog :: log(datalog_type_t data)
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
        dataFile.println("x,\ty,\tz");
        for (uint16_t i = 0; i < SAMPLING ; i++)
        {
            dataFile.print(data.x[i]);
            dataFile.print(",\t");
            dataFile.print(data.y[i]);
            dataFile.print(",\t");
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
