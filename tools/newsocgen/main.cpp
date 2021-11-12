#include "espcreator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (argc != 5)
    {
        printf("usage: ./main <NOC_WIDTH> <TECH_LIB> <MAC_ADDRESS> <FPGA_BOARD>\n");
        return 1;
    }

    espcreator w(NULL, argv[1], argv[2], argv[3], argv[4]);

    w.showMaximized();

    return a.exec();
}
