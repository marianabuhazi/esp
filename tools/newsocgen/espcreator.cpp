#include "espcreator.h"
#include "ui_espcreator.h"
#include "constants.h"
#include <QDebug>

/* ------------------------------------------------------------------------- */
/* -------------------------------- Classes -------------------------------- */
/* ------------------------------------------------------------------------- */

//
// Constructor
//
espcreator::espcreator(QWidget *parent,
                       std::string noc_width, // not used, defined internally using combo_arch_to_nocw
                       std::string tech_library, // TODO: check usage of three args
                       std::string mac_address,
                       std::string board)
    : QMainWindow(parent), ui(new Ui::espcreator)
{
    (void) noc_width;
    NOCX = 0;
    NOCY = 0;
    cpu_arch = TOSTRING(CPU_ARCH);
    std::string mac_addr = get_MAC_Addr(mac_address);
    std::string fpga_board = board;

    ui->setupUi(this);
    ui->lineEdit_mac->setText(mac_addr.c_str());
    ui->lineEdit_mac->setReadOnly(false);
    ui->lineEdit_tech->setText(tech_library.c_str());
    ui->lineEdit_board->setText(TOSTRING(BOARD));
    ui->spinBox_vf->setMinimum(1);
    ui->spinBox_vf->setValue(4);

    ui->combo_slm->setEditable(true);
    ui->combo_slm->lineEdit()->setReadOnly(true);
    ui->combo_slm->lineEdit()->setAlignment(Qt::AlignCenter);
    for (unsigned i = 0; i < slm_kb_per_tile.size(); i++)
    {
        ui->combo_slm->addItem(slm_kb_per_tile[i].c_str());
    }
    ui->combo_slm->setCurrentIndex(slm_kb_per_tile_default);

    ui->combo_data->setEditable(true);
    ui->combo_data->lineEdit()->setReadOnly(true);
    ui->combo_data->lineEdit()->setAlignment(Qt::AlignCenter);
    for (unsigned i = 0; i < data_allocation_strategy.size(); i++)
    {
        ui->combo_data->addItem(data_allocation_strategy[i].c_str());
    }
    ui->combo_data->setCurrentIndex(data_allocation_strategy_default);

    for (unsigned i = 0; i < combo_arch_to_nocw.size(); i++)
    {
        ui->combo_arch->addItem(get_nocw(i, 0).c_str());
    }

    ui->combo_arch->setEditable(true);
    ui->combo_arch->lineEdit()->setReadOnly(true);
    ui->combo_arch->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->combo_arch->setCurrentIndex(combo_arch_default);
    ui->lineEdit_nocw->setText(get_nocw(combo_arch_default, 1).c_str());
    ui->lineEdit_espmac->setText(get_ESP_MAC().c_str());
    ui->lineEdit_espmac->setEnabled(true);
    ui->lineEdit_espip->setText(get_ESP_IP().c_str());
    ui->lineEdit_espip->setEnabled(true);

    for (unsigned i = 0; i < l2_ways.size(); i++)
    {
        ui->combo_l2_ways->addItem(l2_ways[i].c_str());
    }
    ui->combo_l2_ways->setCurrentIndex(l2_ways_default);
    ui->combo_l2_ways->setEnabled(false);

    for (unsigned i = 0; i < l2_sets.size(); i++)
    {
        ui->combo_l2_sets->addItem(l2_sets[i].c_str());
    }
    ui->combo_l2_sets->setCurrentIndex(l2_sets_default);
    ui->combo_l2_sets->setEnabled(false);

    for (unsigned i = 0; i < llc_ways.size(); i++)
    {
        ui->combo_llc_ways->addItem(llc_ways[i].c_str());
    }
    ui->combo_llc_ways->setCurrentIndex(llc_ways_default);
    ui->combo_llc_ways->setEnabled(false);

    for (unsigned i = 0; i < llc_sets.size(); i++)
    {
        ui->combo_llc_sets->addItem(llc_sets[i].c_str());
    }
    ui->combo_llc_sets->setCurrentIndex(llc_sets_default);
    ui->combo_llc_sets->setEnabled(false);

    for (unsigned i = 0; i < al2_ways.size(); i++)
    {
        ui->combo_al2_ways->addItem(al2_ways[i].c_str());
    }
    ui->combo_al2_ways->setCurrentIndex(al2_ways_default);
    ui->combo_al2_ways->setEnabled(false);

    for (unsigned i = 0; i < al2_sets.size(); i++)
    {
        ui->combo_al2_sets->addItem(al2_sets[i].c_str());
    }
    ui->combo_al2_sets->setCurrentIndex(al2_sets_default);
    ui->combo_al2_sets->setEnabled(false);

    for (unsigned i = 0; i < implem.size(); i++)
    {
        ui->combo_implem->addItem(implem[i].c_str());
    }
    ui->combo_implem->setCurrentIndex(0);
    ui->combo_implem->setEnabled(false);

    ui->pushButton_gen->setEnabled(true);

    // UART
    ui->stackedWidget_peripheral_uart->setCurrentIndex(1);

    // JTAG
    ui->stackedWidget_peripheral_jtag->setCurrentIndex(0);
    
    // SVGA
    ui->checkBox_peripheral_svga->setChecked(false);
    if (fpga_board.find("prof_pga") != std::string::npos)
    {
        ui->stackedWidget_peripheral_svga->setCurrentIndex(1);
    } else {
        ui->stackedWidget_peripheral_svga->setCurrentIndex(0);
    }

    read_config(true);
}

//
// Destructor
//
espcreator::~espcreator()
{
    delete ui;
}

/* ------------------------------------------------------------------------- */
/* --------------------------------- Slots --------------------------------- */
/* ------------------------------------------------------------------------- */

//
// Update NoC
//
void espcreator::on_pushButton_noc_clicked()
{
    unsigned new_nocy = ui->spinBox_nocy->value();
    unsigned new_nocx = ui->spinBox_nocx->value();

    // Delete rows
    if (new_nocy < NOCY)
    {
        for (int y = frame_tile.size() - 1; y >= (int)new_nocy; y--)
        {
            for (int x = frame_tile[y].size() - 1; x >= 0; x--)
            {
                delete frame_tile[y][x];
                frame_tile[y].pop_back();
            }
            frame_tile.pop_back();
        }
    }
    // Delete columns
    if (new_nocx < NOCX)
    {
        for (int y = 0; y < (int)frame_tile.size(); y++)
            for (int x = frame_tile[y].size() - 1; x >= (int)new_nocx; x--)
            {
                delete frame_tile[y][x];
                frame_tile[y].pop_back();
            }
    }
    // Add columns
    if (new_nocx > NOCX)
        for (int y = 0; y < (int)frame_tile.size(); y++)
            for (int x = frame_tile[y].size(); x < (int)new_nocx; x++)
                frame_tile[y]
                    .push_back(new Tile(ui->frame_noc, ui->layout_noc, y, x, cpu_arch));

    // Add rows
    if (new_nocy > NOCY)
        for (int y = frame_tile.size(); y < (int)new_nocy; y++)
        {
            frame_tile.push_back(std::vector<Tile *>());
            for (int x = 0; x < (int)new_nocx; x++)
                frame_tile[y]
                    .push_back(new Tile(ui->frame_noc, ui->layout_noc, y, x, cpu_arch));
        }

    for (int y = 0; y < (int)frame_tile.size(); y++)
    {
        for (int x = 0; x < (int)frame_tile[y].size(); x++)
        {
            frame_tile[y][x]->set_id(y * new_nocx + x);
            frame_tile[y][x]->set_vf_points_count(ui->spinBox_vf->value());
        }
    }

    NOCX = new_nocx;
    NOCY = new_nocy;
}

//
// Generate SoC Configuration File
//
// TODO: need to finish error checking
void espcreator::on_pushButton_gen_clicked()
{
    // Error checking
    bool gen_ok = true;
    std::string errors_s = "";

    // update_msg
    int tot_tiles = NOCX * NOCY;
    int tot_acc = 0;
    int tot_cpu = 0;
    int tot_mem = 0;
    int tot_io = 0;
    int tot_slm = 0;
    for (unsigned int y = 0; y < NOCY; y++)
    {
        for (unsigned int x = 0; x < NOCX; x++)
        {
            if (frame_tile[y][x]->get_ip() == "acc")
                tot_acc++;
            if (frame_tile[y][x]->get_ip() == "cpu")
                tot_cpu++;
            if (frame_tile[y][x]->get_ip() == "mem")
                tot_mem++;
            if (frame_tile[y][x]->get_ip() == "misc")
                tot_io++;
            if (frame_tile[y][x]->get_ip() == "slm")
                tot_slm++;
        }
    }
    int cache_en = ui->checkBox_caches->isChecked();
    int tot_full_coherent = 0;
    int tot_llc_coherent = 0;
    if (cache_en)
    {
        // tot_full_coherent =   + tot_cpu; TODO: fix
        tot_llc_coherent = tot_acc;
    }
    int tot_clkbuf; // TODO: fix
    QString slm_q = ui->combo_slm->currentText();
    std::string slm_s = slm_q.toUtf8().constData();
    int slm_kbytes = atoi(slm_s.c_str());
    int tot_slm_size = tot_slm * slm_kbytes;
    int regions; // TODO: fix
    int socTECH; // TODO: fix
    int llc_ways;
    int llc_sets;
    QString cpu_arch_q = ui->combo_arch->currentText();
    std::string cpu_arch_s = cpu_arch_q.toUtf8().constData();
    int clk_region_skip;

    // TODO: number refers to line number in cryo-ai branch
    // 616 NOCY > 8 or NOCX > 8
    if (NOCY > 8 or NOCX > 8)
    {
        gen_ok = false;
        errors_s += "Maximum number of rows and columns is 8\n";
    }

    // 618 tot_cpu == 0
    if (tot_cpu == 0)
    {
        gen_ok = false;
        errors_s += "At least one CPU is required\n";
    }

    // 620 tot_cpu > 1 and cache_en
    if (tot_cpu > 1 and !cache_en)
    {
        gen_ok = false;
        errors_s += "Caches are required for multicore SoCs\n";
    }

    // 622 tot_io == 0
    if (tot_io == 0)
    {
        gen_ok = false;
        errors_s += "At least one I/O tile is required\n";
    }

    // 624 tot_cpu > NCPU_MAX
    if (tot_cpu > NCPU_MAX)
    {
        gen_ok = false;
        errors_s += "Maximum number of supported CPUs is " + to_string(NCPU_MAX) + "\n";
    }

    // 626 tot_io > 1
    if (tot_io > 1)
    {
        gen_ok = false;
        errors_s += "Multiple I/O tiles are not supported\n";
    }

    // 628 tot_mem < 1 and tot_slm < 1
    if (tot_mem < 1 and tot_slm < 1)
    {
        gen_ok = false;
        errors_s += "There must be at least 1 memory tile or 1 SLM tile and no more than " + to_string(NMEM_MAX) + "\n";
    }

    // 630 tot_mem > NMEM_MAX TODO: fix typo
    /*
    if (tot_mem > NMEM_MAX)
    {
        gen_ok = false;
        errors_s += "There must be no more than " + to_string(NMEM_MAX) + "\n";
    }
    */

    // 632 tot_mem == 0 and CPU_ARCH != "ibex" TODO: fix

    // 634 tot_mem == 0 and cache_en == 1
    if (tot_mem == 0 and cache_en)
    {
        gen_ok = false;
        errors_s += "There must be at least 1 memory tile to enable the ESP cache hierarchy\n";
    }
    
    // 636 tot_mem == 2^i
    int tot_mem_tmp = tot_mem;
    while (tot_mem_tmp % 2 == 0 and tot_mem_tmp > 0)
        tot_mem_tmp /= 2;
    if (tot_mem_tmp != 1 and tot_mem_tmp != 0)
    {
        gen_ok = false;
        errors_s += "Number of memory tiles must be a power of 2\n";
    }

    // 638 tot_slm > NSLM_MAX TODO: fix typo
    if (tot_slm > NSLM_MAX)
    {
        gen_ok = false;
        errors_s += "There must be no more than " + to_string(NSLM_MAX) + "\n";
    }

    // 640 tot_slm > 1 and slm_kbytes < 1024
    if (tot_slm > 1 and slm_kbytes < 1024)
    {
        gen_ok = false;
        errors_s += "SLM size must be 1024 KB or more if placing more than one SLM tile\n";
    }


    // 642 TECH != "gf12" and TECH != "virtexu" and tot_mem == 4 TODO: fix

    // 644 llc_sets >= 8192 and llc_ways >= 16 and tot_mem == 1 TODO: fix

    // 646 TECH == "virtexu" and tot_mem >= 2 and (NOCX < 3 or NOCY < 3) TODO: fix
    
    // 648 tot_acc > NACC_MAX TODO: fix typo
    if (tot_acc > NACC_MAX)
    {
        gen_ok = false;
        errors_s += "There must no more than " + to_string(NACC_MAX) + " (can be relaxed)\n";
    }

    // 650 tot_tiles > NTILE_MAX
    if (tot_tiles > NTILE_MAX)
    {
        gen_ok = false;
        errors_s += "Maximum number of supported tiles is " + to_string(NTILE_MAX) + "\n";
    }

    // 652 tot_full_coherent > NFULL_COHERENT_MAX
    if (tot_full_coherent > NFULL_COHERENT_MAX)
    {
        gen_ok = false;
        errors_s += "Maximum number of supported fully-coherent devices is " + to_string(NFULL_COHERENT_MAX) + "\n";
    }

    // 654 tot_llc_coherent > NLLC_COHERENT_MAX
    if (tot_llc_coherent > NLLC_COHERENT_MAX)
    {
        gen_ok = false;
        errors_s += "Maximum number of supported LLC_coherent devices is " + to_string(NLLC_COHERENT_MAX) + "\n";
    }

    // 656 tot_clkbuf > 9 TODO: fix

    // 660 clk_region_skip > 0 TODO: fix

    if (!gen_ok)
    {
        QString errors_q = QString::fromUtf8(errors_s.c_str());
        QMessageBox::critical(this, "Errors", errors_q);
        return;
    }

    // MessageBox
    QMessageBox msgBox;
    msgBox.setText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret)
    {
        case QMessageBox::Save:
            break;
        case QMessageBox::Cancel:
            return;
        default:
            break;
    }
    write_config();
}

//
// NOCX Spinbox
//
void espcreator::on_spinBox_nocx_valueChanged(int arg1 __attribute__((unused)))
{
    check_enable_noc_update();
}

//
// NOCY Spinbox
//
void espcreator::on_spinBox_nocy_valueChanged(int arg1 __attribute__((unused)))
{
    check_enable_noc_update();
}

// 
// VF Points Spinbox
//
void espcreator::on_spinBox_vf_valueChanged(int arg1 __attribute__((unused)))
{
    for (unsigned int y = 0; y < NOCY; y++)
        for (unsigned int x = 0; x < NOCX; x++)
            frame_tile[y][x]->set_vf_points_count(ui->spinBox_vf->value());
}

//
// Enable Caches Checkbox
//
void espcreator::on_checkBox_caches_toggled(bool arg1)
{
    if (arg1 == false)
    {
        ui->combo_l2_ways->setEnabled(false);
        ui->combo_l2_sets->setEnabled(false);
        ui->combo_llc_ways->setEnabled(false);
        ui->combo_llc_sets->setEnabled(false);
        ui->combo_al2_ways->setEnabled(false);
        ui->combo_al2_sets->setEnabled(false);
        ui->combo_implem->setEnabled(false);
    }
    else
    {
        ui->combo_l2_ways->setEnabled(true);
        ui->combo_l2_sets->setEnabled(true);
        ui->combo_llc_ways->setEnabled(true);
        ui->combo_llc_sets->setEnabled(true);
        ui->combo_al2_ways->setEnabled(true);
        ui->combo_al2_sets->setEnabled(true);
        ui->combo_implem->setEnabled(true);
    }
}

//
// CPU Architecture Dropdown Menu
//
void espcreator::on_combo_arch_currentIndexChanged(const QString &arg1)
{
    for (unsigned i = 0; i < combo_arch_to_nocw.size(); i++)
    {
        if (arg1.toStdString() == combo_arch_to_nocw[i][0])
        {
            ui->lineEdit_nocw->setText(get_nocw(i, 1).c_str());
        }
    }
}

/* ------------------------------------------------------------------------- */
/* ------------------------------- Functions ------------------------------- */
/* ------------------------------------------------------------------------- */

//
// Check for valid NOCX and NOCY
//
void espcreator::check_enable_noc_update()
{
    if (ui->spinBox_nocx->value() > 0 && ui->spinBox_nocy->value() > 0)
        ui->pushButton_noc->setEnabled(true);
    else
        ui->pushButton_noc->setEnabled(false);
}

//
// Check if tile is present
//
bool espcreator::check_present(tile_t type)
{
    FOREACH_TILE(y, x)
    if (frame_tile[y][x]->type == type)
        return true;
    return false;
}

//
// Check if tile is present but fewer than max_count
//
bool espcreator::check_present(tile_t type, unsigned max_count)
{
    unsigned count = 0;
    for (int y = 0; y < (int)frame_tile.size(); y++)
        for (int x = 0; x < (int)frame_tile[y].size(); x++)
            if (frame_tile[y][x]->type == type)
                count++;
    if (count > 0 && count <= max_count)
        return true;
    return false;
}

//
// Check Clock Domains
//
bool espcreator::check_clock_domains()
{
    std::map<int, int> domain;
    unsigned domain_count = 0;

    FOREACH_TILE(y, x)
    {
        int inc = frame_tile[y][x]->has_pll ? 1 : 0;
        if (frame_tile[y][x]->domain != 0)
        {
            if (domain.insert(std::make_pair(frame_tile[y][x]->domain, inc)).second ==
                false)
                domain[frame_tile[y][x]->domain] = domain[frame_tile[y][x]->domain] + inc;
            else
                domain_count++;
            if (domain[frame_tile[y][x]->domain] > 1)
                return false;
        }
    }

    FOREACH_TILE(y, x)
    {
        if (frame_tile[y][x]->domain != 0 && domain[frame_tile[y][x]->domain] != 1)
            return false;
        if (frame_tile[y][x]->domain > domain_count)
            return false;
    }

    return true;
}

//
// Read Configuration File
//
int espcreator::read_config(bool temporary)
{
    std::string filename = ".esp_config";
    bool warning = false;
    if (temporary)
    {
        filename = get_esp_config_bak();
        warning = true;
        if (!isfile(filename))
        {
            filename = ".esp_config";
            warning = false;
            if (!isfile(filename))
            {
                return -1;
            }
        }
    }
    if (!isfile(filename))
    {
        std::cout << "Configuration file is not available\n";
        return -1;
    }
    if (warning)
    {
        bool first = true;
        if (isfile(".esp_config"))
        {
            std::fstream orig;
            std::fstream bak;
            orig.open(".esp_config", std::ios::in);
            bak.open(get_esp_config_bak(), std::ios::in);

            std::string line_orig;
            std::string line_bak;
            while(std::getline(bak, line_bak))
            {
                std::getline(orig, line_orig);
                if (line_bak != line_orig)
                {
                    if (first)
                    {
                        std::cout << "WARNING: temporary configuration. Modifications are not reported into 'socmap.vhd' yet\n";
                        first = false;
                    }
                    str_erase(line_orig, '\n');
                    str_erase(line_bak, '\n');
                    std::cout << "SAVED: " << line_orig << " -- TEMP: " << line_bak << "\n";
                }
            }
            orig.close();
            bak.close();
        }
    }
    std::fstream fp;
    fp.open(filename, std::ios::in);
    // CPU architecture
    std::string line;
    std::getline(fp, line);
    std::vector<std::string> item;
    item = str_split(line, ' ');
    this->cpu_arch = item[2];
    QString cpu_arch_q = QString::fromStdString(this->cpu_arch);
    ui->combo_arch->setCurrentText(cpu_arch_q);
    // CPU count (skip this info while rebuilding SoC config)
    std::getline(fp, line);
    // Scatter-gather
    std::getline(fp, line);
    if (line.find("CONFIG_HAS_SG = y") != std::string::npos)
    {
        // self.HAS_SG = True TODO: what is this? line 147 soc.py
        ui->combo_data->setCurrentIndex(1);
    } else {
        ui->combo_data->setCurrentIndex(0);
    }
    // Topology
    std::getline(fp, line);
    item = str_split(line, ' ');
    int rows;
    rows = stoi(item[2]);
    std::getline(fp, line);
    item = str_split(line, ' ');
    int cols;
    cols = stoi(item[2]);
    // create_topology
    // Add columns
    for (int y = 0; y < (int)frame_tile.size(); y++)
        for (int x = frame_tile[y].size(); x < cols; x++)
            frame_tile[y]
                .push_back(new Tile(ui->frame_noc, ui->layout_noc, y, x, cpu_arch));

    // Add rows
    for (int y = frame_tile.size(); y < rows; y++)
    {
        frame_tile.push_back(std::vector<Tile *>());
        for (int x = 0; x < cols; x++)
            frame_tile[y]
                .push_back(new Tile(ui->frame_noc, ui->layout_noc, y, x, cpu_arch));
    }
    for (int y = 0; y < (int)frame_tile.size(); y++)
    {
        for (int x = 0; x < (int)frame_tile[y].size(); x++)
        {
            frame_tile[y][x]->set_id(y * cols + x);
            frame_tile[y][x]->set_vf_points_count(ui->spinBox_vf->value());
        }
    }
    NOCX = cols;
    NOCY = rows;
    ui->spinBox_nocy->setValue(NOCY);
    ui->spinBox_nocx->setValue(NOCX);
    // CONFIG_CPU_CACHES = L2_SETS L2_WAYS LLC_SETS LLC_WAYS
    std::getline(fp, line);
    if (line.find("CONFIG_CACHE_EN = y") != std::string::npos)
        ui->checkBox_caches->setChecked(true);
    else
        ui->checkBox_caches->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_CACHE_RTL = y") != std::string::npos)
        ui->combo_implem->setCurrentText("ESP RTL");
    else
        ui->combo_implem->setCurrentText("ESP HLS");
    // addition for CONFIG_CACHE_SPANDEX
    std::getline(fp, line);
    if (line.find("CONFIG_CACHE_SPANDEX = y") != std::string::npos)
        ui->combo_implem->setCurrentText("SPANDEX HLS");

    std::getline(fp, line);
    item = str_split(line, ' ');
    ui->combo_l2_sets->setCurrentText(QString::fromStdString(item[2] + " sets"));
    ui->combo_l2_ways->setCurrentText(QString::fromStdString(item[3] + " ways"));
    ui->combo_llc_sets->setCurrentText(QString::fromStdString(item[4] + " sets"));
    ui->combo_llc_ways->setCurrentText(QString::fromStdString(item[5] + " ways"));
    // CONFIG_ACC_CACHES = ACC_L2_SETS ACC_L2_WAYS
    std::getline(fp, line);
    item = str_split(line, ' ');
    ui->combo_al2_sets->setCurrentText(QString::fromStdString(item[2] + " sets"));
    ui->combo_al2_ways->setCurrentText(QString::fromStdString(item[3] + " ways"));
    // CONFIG_SLM_KBYTES
    std::getline(fp, line);
    item = str_split(line, ' ');
    ui->combo_slm->setCurrentText(QString::fromStdString(item[2]));
    // Ethernet
    std::getline(fp, line);
    if (line.find("CONFIG_ETH_EN = y") != std::string::npos)
        ui->checkBox_peripheral_ethernet->setChecked(true);
    else
        ui->checkBox_peripheral_ethernet->setChecked(false);
    // SVGA
    std::getline(fp, line);
    if (line.find("CONFIG_SVGA_EN = y") != std::string::npos)
        ui->checkBox_peripheral_svga->setChecked(true);
    else
        ui->checkBox_peripheral_svga->setChecked(false);
    // Debug Link
    std::getline(fp, line);
    item = str_split(line, ' ');
    std::string dsu_ip = item[2];
    ui->lineEdit_espip->setText(QString::fromUtf8(dsu_ip.c_str()));
    std::getline(fp, line);
    item = str_split(line, ' ');
    std:: string dsu_eth = item[2];
    ui->lineEdit_espmac->setText(QString::fromUtf8(dsu_eth.c_str()));
    // Monitors
    std::getline(fp, line);
    if (line.find("CONFIG_MON_DDR = y") != std::string::npos)
        ui->checkBox_probe_mem_band->setChecked(true);
    else
        ui->checkBox_probe_mem_band->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_MON_MEM = y") != std::string::npos)
        ui->checkBox_probe_mem->setChecked(true);
    else
        ui->checkBox_probe_mem->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_MON_INJ = y") != std::string::npos)
        ui->checkBox_probe_inject->setChecked(true);
    else
        ui->checkBox_probe_inject->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_MON_ROUTERS = y") != std::string::npos)
        ui->checkBox_probe_noc_routers->setChecked(true);
    else
        ui->checkBox_probe_noc_routers->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_MON_ACCELERATORS = y") != std::string::npos)
        ui->checkBox_probe_acc->setChecked(true);
    else
        ui->checkBox_probe_acc->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_MON_L2 = y") != std::string::npos)
        ui->checkBox_probe_l2->setChecked(true);
    else
        ui->checkBox_probe_l2->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_MON_LLC = y") != std::string::npos)
        ui->checkBox_probe_llc->setChecked(true);
    else
        ui->checkBox_probe_llc->setChecked(false);
    std::getline(fp, line);
    if (line.find("CONFIG_MON_DVFS = y") != std::string::npos)
        ui->checkBox_probe_dvfs->setChecked(true);
    else
        ui->checkBox_probe_dvfs->setChecked(false);
    // Tiles configuration
    std::vector<std::string> tokens;
    for (unsigned int y = 0; y < NOCY; y++)
    {
        for (unsigned int x = 0; x < NOCX; x++)
        {
            std::getline(fp, line);
            str_erase(line, '\n');
            tokens = str_split(line, ' ');
            if (tokens.size() > 1)
            {
                frame_tile[y][x]->set_type(tokens[4]);
                frame_tile[y][x]->set_domain(tokens[5]);
                frame_tile[y][x]->set_pll(tokens[6]);
                frame_tile[y][x]->set_buf(tokens[7]);
                int cache_en = ui->checkBox_caches->isChecked();
                if ((tokens[3] == "cpu") and (cache_en))
                    frame_tile[y][x]->set_cache("1");
                if (tokens[3] == "acc")
                {
                    frame_tile[y][x]->set_acc_l2(tokens[9]);
                    frame_tile[y][x]->set_ip(tokens[4]);
                    frame_tile[y][x]->set_impl(tokens[4] + "_" + tokens[8]);
                    frame_tile[y][x]->set_impl(tokens[8]);
                }
            }
        }
    }
    std::getline(fp, line);
    std::getline(fp, line);
    item = str_split(line, ' ');
    int vf_points;
    vf_points = stoi(item[2]);
    ui->spinBox_vf->setValue(vf_points);
    // Power annotation
    for (unsigned int y = 0; y < NOCY; y++)
    {
        for (unsigned int x = 0; x < NOCX; x++)
        {
            std::getline(fp, line);
            str_erase(line, '\n');
            if (line.length() == 0)
                return 0;
            tokens = str_split(line, ' ');
            // tile.create_characterization(self, self.noc.vf_points) self.noc.vf_points = ui->spinBox_vf TODO: fix?
            if (frame_tile[y][x]->get_ip() == tokens[2])
            {
                for (int vf = 0; vf < vf_points; vf++)
                {
                    frame_tile[y][x]->set_vf_spin_boxes(vf * 3, tokens[3 + vf * 3]);
                    frame_tile[y][x]->set_vf_spin_boxes(vf * 3 + 1, tokens[3 + vf * 3 + 1]);
                    frame_tile[y][x]->set_vf_spin_boxes(vf * 3 + 2, tokens[3 + vf * 3 + 2]);
                }
            }
        }
    }
    return 0;
}

// Write configuration
int espcreator::write_config()
{
    // Determine backup configuration file name.
    int i = 1;
    std::string cfg_file_name = ".esp_config";
    while (true)
    {
        std::string bkp_file_name = cfg_file_name + ".bak." + to_string(i);
        std::ifstream bkp(bkp_file_name.c_str());
        if (!bkp.good())
            break;
        i++;
    }
    std::string bkp_file_name = cfg_file_name + ".bak." + to_string(i);

    // Write new configuration
    std::cerr << bkp_file_name.c_str() << "\n";
    std::ofstream fp(bkp_file_name.c_str());

    // CPU_ARCH
    QString cpu_arch_q = ui->combo_arch->currentText();
    std::string cpu_arch_s = cpu_arch_q.toUtf8().constData();
    fp << "CPU_ARCH = " << cpu_arch_s.c_str() << "\n";

    // NCPU_TILE
    int tot_cpu = 0;
    for (unsigned int y = 0; y < NOCY; y++)
    {
        for (unsigned int x = 0; x < NOCX; x++)
        {
            if (frame_tile[y][x]->get_ip() == "cpu")
                tot_cpu++;
        }
    }
    fp << "NCPU_TILE = ";
    fp << to_string(tot_cpu) << "\n";

    // CONFIG_HAS_SG
    QString combo_slm_q = ui->combo_data->currentText();
    std::string combo_slm_s = combo_slm_q.toUtf8().constData();
    std::string scatter_gather ("Scatter/Gather");
    if (!combo_slm_s.compare(scatter_gather))
        fp << "CONFIG_HAS_SG = y\n";
    else
        fp << "#CONFIG_HAS_SG is not set\n";

    // CONFIG_NOC_ROWS
    fp << "CONFIG_NOC_ROWS = " << to_string(NOCY) << "\n";

    // CONFIG_NOC_COLS
    fp << "CONFIG_NOC_COLS = " << to_string(NOCX) << "\n";

    // CONFIG_CACHE_EN
    int cache_en = ui->checkBox_caches->isChecked();
    if (cache_en)
        fp << "CONFIG_CACHE_EN = y\n";
    else
        fp << "#CONFIG_CACHE_EN is not set\n";

    // CONFIG_CACHE_RTL
    QString combo_implem_q = ui->combo_implem->currentText();
    std::string combo_implem_s = combo_implem_q.toUtf8().constData();
    bool config_cache_rtl_set = false;
    for (unsigned int i = 0; i < implem_to_RTL.size(); i++)
    {
        if (combo_implem_s == implem_to_RTL[i][0] and !config_cache_rtl_set)
        {
            if (implem_to_RTL[i][1] == "y")
                fp << "CONFIG_CACHE_RTL = y\n";
            else
                fp << "#CONFIG_CACHE_RTL is not set\n";
            config_cache_rtl_set = true;
        }
    }
    if (!config_cache_rtl_set)
        fp << "#CONFIG_CACHE_RTL is not set\n";

    // CONFIG_CACHE_SPANDEX
    if (combo_implem_s == "SPANDEX HLS")
        fp << "CONFIG_CACHE_SPANDEX = y\n";
    else
        fp << "#CONFIG_CACHE_SPANDEX is not set\n";

    // CONFIG_CPU_CACHES
    QString l2_sets_q = ui->combo_l2_sets->currentText();
    std::string l2_sets_s = l2_sets_q.toUtf8().constData();
    std::string l2_sets_sf = l2_sets_s.substr(0, l2_sets_s.find(" "));
    QString l2_ways_q = ui->combo_l2_ways->currentText();
    std::string l2_ways_s = l2_ways_q.toUtf8().constData();
    std::string l2_ways_sf = l2_ways_s.substr(0, l2_ways_s.find(" "));
    fp << "CONFIG_CPU_CACHES = " << l2_sets_sf.c_str() << " " << l2_ways_sf.c_str() << " ";
    QString llc_sets_q = ui->combo_llc_sets->currentText();
    std::string llc_sets_s = llc_sets_q.toUtf8().constData();
    std::string llc_sets_sf = llc_sets_s.substr(0, llc_sets_s.find(" "));
    QString llc_ways_q = ui->combo_llc_ways->currentText();
    std::string llc_ways_s = llc_ways_q.toUtf8().constData();
    std::string llc_ways_sf = llc_ways_s.substr(0, llc_ways_s.find(" "));
    fp << llc_sets_sf.c_str() << " " << llc_ways_sf.c_str() << "\n";

    // CONFIG_ACC_CACHES
    QString al2_sets_q = ui->combo_al2_sets->currentText();
    std::string al2_sets_s = al2_sets_q.toUtf8().constData();
    std::string al2_sets_sf = al2_sets_s.substr(0, al2_sets_s.find(" "));
    QString al2_ways_q = ui->combo_al2_ways->currentText();
    std::string al2_ways_s = al2_ways_q.toUtf8().constData();
    std::string al2_ways_sf = al2_ways_s.substr(0, al2_ways_s.find(" "));
    fp << "CONFIG_ACC_CACHES = " << al2_sets_sf.c_str() << " " << al2_ways_sf.c_str() << "\n";

    // CONFIG_SLM_KBYTES
    QString slm_q = ui->combo_slm->currentText();
    std::string slm_s = slm_q.toUtf8().constData();
    fp << "CONFIG_SLM_KBYTES = " << slm_s.c_str() << "\n";

    // CONFIG_ETH_EN
    if (ui->checkBox_peripheral_ethernet->isChecked())
        fp << "CONFIG_ETH_EN = y\n";
    else
        fp << "#CONFIG_ETH_EN is not set\n";
    
    // CONFIG_SVGA_EN
    if (ui->checkBox_peripheral_svga->isChecked())
        fp << "CONFIG_SVGA_EN = y\n";
    else
        fp << "#CONFIG_SVGA_EN is not set\n";
    
    // CONFIG_DSU_IP
    fp << "CONFIG_DSU_IP = " << ui->lineEdit_espip->text().toUtf8().constData() << "\n";

    // CONFIG_DSU_ETH
    fp << "CONFIG_DSU_ETH = " << ui->lineEdit_espmac->text().toUtf8().constData() << "\n";

    // CONFIG_MON_DDR
    if (ui->checkBox_probe_mem_band->isChecked())
        fp << "CONFIG_MON_DDR = y\n";
    else
        fp << "#CONFIG_MON_DDR is not set\n";

    // CONFIG_MON_MEM
    if (ui->checkBox_probe_mem->isChecked())
        fp << "CONFIG_MON_MEM = y\n";
    else
        fp << "#CONFIG_MON_MEM is not set\n";

    // CONFIG_MON_INJ
    if (ui->checkBox_probe_inject->isChecked())
        fp << "CONFIG_MON_INJ = y\n";
    else
        fp << "#CONFIG_MON_INJ is not set\n";
    
    // CONFIG_ROUTERS
    if (ui->checkBox_probe_noc_routers->isChecked())
        fp << "CONFIG_MON_ROUTERS = y\n";
    else
        fp << "#CONFIG_MON_ROUTERS is not set\n";
    
    // CONFIG_ACCELERATORS
    if (ui->checkBox_probe_acc->isChecked())
        fp << "CONFIG_MON_ACCELERATORS = y\n";
    else
        fp << "#CONFIG_MON_ACCELERATORS is not set\n";

    // CONFIG_MON_L2
    if (ui->checkBox_probe_l2->isChecked())
        fp << "CONFIG_MON_L2 = y\n";
    else
        fp << "#CONFIG_MON_L2 is not set\n";

    // CONFIG_MON_LLC
    if (ui->checkBox_probe_llc->isChecked())
        fp << "CONFIG_MON_LLC = y\n";
    else
        fp << "#CONFIG_MON_LLC is not set\n";

    // CONFIG_MON_DVFS
    if (ui->checkBox_probe_dvfs->isChecked())
        fp << "CONFIG_MON_DVFS = y\n";
    else
        fp << "#CONFIG_MON_DVFS is not set\n";

    // TILES
    for (unsigned int y = 0; y < NOCY; y++)
    {
        for (unsigned int x = 0; x < NOCX; x++)
        {
            fp << "TILE_" << to_string(y) << "_" << to_string(x) << " = ";
            fp << to_string(y * NOCX + x) << " ";
            fp << frame_tile[y][x]->get_type() << " ";
            if (frame_tile[y][x]->get_type() == "misc")
                fp << "IO ";
            else if (frame_tile[y][x]->get_type() == "acc")
                fp << frame_tile[y][x]->get_ip_acc() << " ";
            else
                fp << frame_tile[y][x]->get_ip() << " ";
            fp << frame_tile[y][x]->get_domain() << " ";
            fp << frame_tile[y][x]->get_PLL() << " ";
            fp << frame_tile[y][x]->get_buf();
            if (frame_tile[y][x]->get_type() == "slm")
            {
                fp << " " << frame_tile[y][x]->get_has_ddr_sel();
            }
            if (frame_tile[y][x]->get_type() == "acc")
            {
                if (frame_tile[y][x]->get_third_party_acc())
                    fp << " " << "impl" << " " << frame_tile[y][x]->get_acc_l2() << " " << frame_tile[y][x]->get_vendor(); // frame_tile[y][x]->get_impl_acc().c_str()
                else
                    fp << " " << frame_tile[y][x]->get_impl_acc().c_str() << " " << frame_tile[y][x]->get_acc_l2() << " sld";
            }
            fp << "\n";
        }
    }

    // CONFIG_HAS_DVFS
    bool nonzero_clock = false;
    for (unsigned int y = 0; y < NOCY; y++)
    {
        for (unsigned int x = 0; x < NOCX; x++)
        {
            if (frame_tile[y][x]->get_domain() != "0")
            {
                nonzero_clock = true;
            }
        }
    }
    if (nonzero_clock)
        fp << "CONFIG_HAS_DVFS = y\n";
    else
        fp << "#CONFIG_HAS_DVFS is not set\n";

    // CONFIG_VF_POINTS TODO: fix
    fp << "CONFIG_VF_POINTS = " << to_string(ui->spinBox_vf->value()) << "\n";

    // POWER
    for (unsigned int y = 0; y < NOCY; y++)
    {
        for (unsigned int x = 0; x < NOCX; x++)
        {
            fp << "POWER_" << to_string(y) << "_" << to_string(x) << " = ";
            if (frame_tile[y][x]->get_type() == "misc")
                fp << "IO ";
            else
                fp << frame_tile[y][x]->get_ip() << " ";
            if (frame_tile[y][x]->get_type() != "acc")
            {
                for (int i = 0; i < ui->spinBox_vf->value(); i++)
                {
                    fp << "0 0 0 ";
                }
            }
            else
            {
                fp << frame_tile[y][x]->get_power();
            }
            fp << "\n";
        }
    }

    fp.close();

    std::string system_1 = "cp ";
    std::string system_2 = bkp_file_name.c_str();
    std::string system_3 = " socgen/esp/.esp_config";
    std::string sys = system_1 + system_2 + system_3;
    if(system(sys.c_str()) < 0)
    {
        return -1;
    }

    QMessageBox msgBox_saved;
    QString saved_msg = "New configuration file saved in " + QString::fromUtf8(bkp_file_name.c_str());
    msgBox_saved.setText(saved_msg);
    msgBox_saved.exec();

    return 0;
}

/* ------------------------------------------------------------------------- */
/* --------------------------- Helper Functions ---------------------------- */
/* ------------------------------------------------------------------------- */

//
// get ESP MAC
//
std::string espcreator::get_ESP_MAC()
{
    std::string str, ethipm, ethipl;
    std::ifstream ifs("grlib_config.vhd");
    while (std::getline(ifs, str))
    {
        if (str.find("CFG_ETH_ENM") != std::string::npos)
        {
            ethipm = str.substr(str.find("16#") + 3, 6);
        }

        if (str.find("CFG_ETH_ENL") != std::string::npos)
        {
            ethipl = str.substr(str.find("16#") + 3, 6);
        }
    }
    str = ethipm + ethipl;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    return str;
}

//
// get ESP IP
//
std::string espcreator::get_ESP_IP()
{
    std::string str, ethipm, ethipl;
    std::ifstream ifs("grlib_config.vhd");
    while (std::getline(ifs, str))
    {
        if (str.find("CFG_ETH_IPM") != std::string::npos)
        {
            ethipm = str.substr(str.find("16#") + 3, 4);
        }

        if (str.find("CFG_ETH_IPL") != std::string::npos)
        {
            ethipl = str.substr(str.find("16#") + 3, 4);
        }
    }

    char buf[20];
    str = ethipm + ethipl;
    int part1 = strtol(str.substr(0, 2).c_str(), NULL, 16);
    int part2 = strtol(str.substr(2, 2).c_str(), NULL, 16);
    int part3 = strtol(str.substr(4, 2).c_str(), NULL, 16);
    int part4 = strtol(str.substr(6, 2).c_str(), NULL, 16);
    sprintf(buf, "%d.%d.%d.%d", part1, part2, part3, part4);

    return std::string(buf);
}

//
// get MAC Addr
//
std::string espcreator::get_MAC_Addr(std::string mac)
{
    int length = mac.length() + 3;
    for (int i = 2; i < length; i += 3)
        mac.insert(i, ":");
    std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
    return mac;
}

//
// get noc width
//
std::string espcreator::get_nocw(int i, int j)
{
    return combo_arch_to_nocw[i][j];
}

//
// get .esp_config.bak file
//
std::string espcreator::get_esp_config_bak()
{
    std::string esp_config_bak = ".esp_config.bak.1";
    return esp_config_bak;
}

//
// split string by delimiter
//
std::vector<std::string> espcreator::str_split(std::string &s, char delimiter)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delimiter)) {
        elems.push_back(item);
        // elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
    }
    return elems;
}

//
// erase character from string
//
void espcreator::str_erase(std::string str, char erase)
{
    str.erase(std::remove(str.begin(), str.end(), erase), str.end());
}

//
// check if is file
//
bool espcreator::isfile(std::string filename)
{
    struct stat sb;
    return (stat(filename.c_str(), &sb) == 0 && S_ISREG(sb.st_mode));
}


