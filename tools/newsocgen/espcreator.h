#ifndef ESPCREATOR_H
#define ESPCREATOR_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <stdio.h>
#include <sys/sendfile.h> // sendfile
#include <fcntl.h>        // open
#include <unistd.h>       // close
#include <sys/stat.h>     // fstat
#include <sys/types.h>    // fstat
#include <ctime>

#include <QMainWindow>
#include <QFrame>
#include <QMessageBox>

#include <tile.h>
#include "esp_constants.h"

using namespace socmap;

namespace Ui
{
class espcreator;
}

#define FOREACH_TILE(_y, _x)                                                             \
    for (int _y = 0; _y < (int)frame_tile.size(); _y++)                                  \
        for (int _x = 0; _x < (int)frame_tile[_y].size(); _x++)

#define REV_FOREACH_POW(_i) for (int _i = (int)frame_power.size() - 1; _i >= 0; _i--)

class espcreator : public QMainWindow
{
    Q_OBJECT

  public:
    espcreator(QWidget *parent,
               std::string noc_width,
               std::string tech_library,
               std::string mac_address,
               std::string board);
    ~espcreator();

  private
slots:
    void on_pushButton_noc_clicked();
    void on_pushButton_gen_clicked();
    void on_spinBox_nocx_valueChanged(int arg1);
    void on_spinBox_nocy_valueChanged(int arg1);
    void on_spinBox_vf_valueChanged(int arg1);
    void on_checkBox_caches_toggled(bool arg1);
    void on_combo_arch_currentIndexChanged(const QString &arg1);

  private:
    // Variables
    Ui::espcreator *ui;
    unsigned int NOCX;
    unsigned int NOCY;
    std::string cpu_arch;
    std::vector<std::vector<Tile *> > frame_tile;

    // Functions
    void check_enable_noc_update();
    void update_power_info();
    bool check_present(tile_t type);
    bool check_present(tile_t type, unsigned max_count);
    bool check_clock_domains();
    int read_config(bool temporary);
    int write_config();

    // Helper Functions
    std::string get_ESP_MAC();
    std::string get_ESP_IP();
    std::string get_MAC_Addr(std::string mac);
    std::string get_nocw(int i, int j);
    std::string get_esp_config_bak();
    std::vector<std::string> str_split(std::string &s, char delimiter);
    void str_erase(std::string str, char erase);
    bool isfile(std::string filename);
};

#endif // ESPCREATOR_H
