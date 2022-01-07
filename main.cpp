#include "CppLinuxSerial/SerialPort.hpp"
#include <QDebug>
#include <QTime>

unsigned char temp[6];
unsigned char data[1];
int n;
int it;

int main() {
//    mn::CppLinuxSerial::SerialPort serialPort("/dev/ttyUSB0", mn::CppLinuxSerial::BaudRate::B_115200);
    mn::CppLinuxSerial::SerialPort serialPort("/dev/ttymxc4", mn::CppLinuxSerial::BaudRate::B_115200);
    serialPort.SetTimeout(-1);
    serialPort.Open();
    QByteArray local_buffer;
    int count = 0;
    QTime time = QTime::currentTime();
    union {
        int16_t number;
        uint8_t array[2];
    } converter;

    while(1){
        // Lendo byte a byte
        QByteArray n = serialPort.ReadBinary(1);

        // Se houver algum byte...
        if(!n.isEmpty())
        {
            // Armazena no buffer de pacote
            local_buffer.append(n);

            // Se o último byte recebido for um final de pacote
            if(n.mid(0,1) == "\x55")
            {
                if(local_buffer.mid(0,1) == "\xaa")
                {
                    converter.array[0] = local_buffer[2];
                    converter.array[1] = local_buffer[1];

                    count++;
//                    if(count == 500){
//                        qDebug() << converter.number/10.0;; // Exibe valor recebido
//                        count = 0;
//                    }
                    int t = time.msecsTo(QTime::currentTime());
                    if(t > 1)
                        qDebug() << "t: " << t;
                    time = QTime::currentTime();
                }
                local_buffer.clear();
            }
        }
    }
}

