// OS COMENTÁRIOS RESERVAM UMA SOLUÇÃO PARA OBTER RSSIS E CALCULAR A MODA,
// NO ENTANTO ESTOU TENDO ERRO DE ESTOURO DE PILHA DE MEMÓRIA

#include <BLEDevice.h>
#include <LinkedList.h>

#define LED_AZUL_SCAN_ATIVO 2
#define QUANTIDADE_ITAGS 5
#define QUANTIDADE_MODA 5
#define SEGUNDOS_SCANNER 1
#define TEMPO_PAUSA 10000

int totalScan;
BLEScan* scanner;
struct Dispositivo {
  int Id;
  //LinkedList<int> Rssis;
  int Moda;
};
LinkedList<Dispositivo> dispositivos = LinkedList<Dispositivo>();
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
  digitalWrite(LED_AZUL_SCAN_ATIVO, HIGH);
  
  acumularRssis();
  definirModas();
  imprimirResultado();
      
  digitalWrite(LED_AZUL_SCAN_ATIVO, LOW);
  delay(TEMPO_PAUSA);
}

void acumularRssis()
{
  do {       
    totalScan++;
    if (totalScan == 1)
      dispositivos.clear();
    BLEScanResults dispositivosEncontrados = realizarScan();
    atualizarLista(dispositivosEncontrados);                        
  } while (totalScan < QUANTIDADE_MODA);  
}

BLEScanResults realizarScan()
{
  BLEScanResults dispositivosEncontrados = scanner->start(SEGUNDOS_SCANNER);
  scanner->stop();
  return dispositivosEncontrados;
}

void atualizarLista(BLEScanResults dispositivosEncontrados)
{
  for(int i = 0; i < dispositivosEncontrados.getCount(); i++) {
    BLEAdvertisedDevice dispositivo = dispositivosEncontrados.getDevice(i);
    for(int j = 0; j < QUANTIDADE_ITAGS; j++) {
      if(dispositivo.getAddress().toString() == iTags[j]) {

        // DISPOSITIVO É UM ITAG
        
        int numeroItag = j + 1;
        Dispositivo existente;
        bool existe = false;
        
        for(int k = 0; k < dispositivos.size(); k++) {
          existente = dispositivos.get(k);
          if (numeroItag == existente.Id) {
            existe = true;
            //LinkedList<int> listaAntiga = existente.Rssis;
            //Dispositivo atualizaElemento = {numeroItag, listaAntiga};
            Dispositivo atualizaElemento = {numeroItag};
            //atualizaElemento.Rssis.add(dispositivo.getRSSI());
            dispositivos.set(k, atualizaElemento);
            break;
          }
        }
        
        if (!existe) {
          //LinkedList<int> novaLista = LinkedList<int>();
          //Dispositivo novoElemento = {numeroItag, novaLista};
          Dispositivo novoElemento = {numeroItag};
          //novoElemento.Rssis.add(dispositivo.getRSSI());
          dispositivos.add(novoElemento);
        }
        
        break;
      }
    }
  }
}

void definirModas() {
  for(int i = 0; i < dispositivos.size(); i++) {
    Dispositivo dispositivo = dispositivos.get(i);
    dispositivo.Moda = -56; // TODO - CALCULAR MODA COM BASE NOS VALORES OBTIDOS
    dispositivos.set(i, dispositivo);
  }
}

void imprimirResultado()
{  
  if (dispositivos.size() <= 0) {      
    Serial.println("--- Nenhuma moda encontrada ---");
    return;
  }

  for(int i = 0; i < dispositivos.size(); i++) {
    Dispositivo dispositivo = dispositivos.get(i);
    Serial.println(" --- ");
    Serial.println(" iTag: " + (String)dispositivo.Id);
    /*Serial.print(" Qtde: (" + (String)dispositivo.Rssis.size() + ") ");

    int qtde = dispositivo.Rssis.size();
    for(int j = 0; j < (qtde - 1); j++)     
      Serial.print((String)dispositivo.Rssis.get(j) + ",");
    Serial.println((String)dispositivo.Rssis.get(qtde - 1));*/
    
    Serial.println(" Moda: " + (String)dispositivo.Moda);
  }
}
