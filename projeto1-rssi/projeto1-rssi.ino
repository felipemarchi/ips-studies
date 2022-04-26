#include <BLEDevice.h>

#define VISUALIZAR_DISPOSITIVOS false
#define LED_AZUL_SCAN_ATIVO 2
#define QUANTIDADE_ITAGS 5
#define SEGUNDOS_SCANNER 1
#define TOTAL_SCAN 100
#define TEMPO_PAUSA 1000

int totalScan;
BLEScan* scanner;
int totalConjuntos = 0;
std::__cxx11::string iTags[QUANTIDADE_ITAGS] = {
    "fc:58:fa:b4:60:96",
    "fc:58:fa:b4:64:2b",
    "fc:58:fa:b3:87:4f",
    "fc:58:fa:b7:5a:4a",
    "fc:58:fa:b3:8b:94"
};

void setup()
{    
    BLEDevice::init("");
    scanner = BLEDevice::getScan();
    scanner->setActiveScan(true);
    
    Serial.begin(115200);
    pinMode(LED_AZUL_SCAN_ATIVO, OUTPUT);
}

void loop()
{
    totalScan = 0;
    totalConjuntos++;
    Serial.println("--- " + String(totalConjuntos) + " ---");
    digitalWrite(LED_AZUL_SCAN_ATIVO, HIGH);
    
    do
    {
        totalScan++;             
        realizarScan();
                
    } while (totalScan < TOTAL_SCAN);
    
    digitalWrite(LED_AZUL_SCAN_ATIVO, LOW);
    delay(TEMPO_PAUSA);
}

void realizarScan()
{
    BLEScanResults dispositivosEncontrados = scanner->start(SEGUNDOS_SCANNER);
    scanner->stop();

    for(int i = 0; i < dispositivosEncontrados.getCount(); i++)
    {
        BLEAdvertisedDevice dispositivo = dispositivosEncontrados.getDevice(i);
        
        if (VISUALIZAR_DISPOSITIVOS)
            Serial.println("Dispositivo: " + String(dispositivo.toString().c_str()));

        for(int j = 0; j < QUANTIDADE_ITAGS; j++)
        {
            if(dispositivo.getAddress().toString() == iTags[j])
            {
                int numeroItag = j + 1;
                if (totalScan < 10)
                  Serial.println(" (" + (String)totalScan + ") " + (String)numeroItag + ": " + (String)dispositivo.getRSSI());
                else if (totalScan < 100)
                  Serial.println("(" + (String)totalScan + ") " + (String)numeroItag + ": " + (String)dispositivo.getRSSI());
                else
                  Serial.println("(" + (String)totalScan + ") " + (String)numeroItag + ":" + (String)dispositivo.getRSSI());
            }
        }
    }
}
