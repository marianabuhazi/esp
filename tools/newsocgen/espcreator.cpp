#include "espcreator.h"
#include "ui_espcreator.h"
#include "constants.h"
#include <QDebug>

//
// Constructor
//

espcreator::espcreator(QWidget *parent,
                       std::string noc_width,
                       std::string tech_library,
                       std::string mac_address)
    : QMainWindow(parent), ui(new Ui::espcreator)
{
    NOCX = 0;
    NOCY = 0;
    cpu_arch = TOSTRING(CPU_ARCH);
    std::string mac_addr = get_MAC_Addr(mac_address);

    ui->setupUi(this);
    ui->lineEdit_mac->setText(mac_addr.c_str());
    ui->lineEdit_tech->setText(tech_library.c_str());
    ui->lineEdit_board->setText(TOSTRING(BOARD));
    ui->spinBox_vf->setMinimum(1);
    ui->spinBox_vf->setValue(4);

    ui->combo_slm->setEditable(true);
    ui->combo_slm->lineEdit()->setReadOnly(true);
    ui->combo_slm->lineEdit()->setAlignment(Qt::AlignCenter);
    for (unsigned i = 0; i < combo_slm.size(); i++)
    {
        ui->combo_slm->addItem(combo_slm[i].c_str());
    }
    ui->combo_slm->setCurrentIndex(combo_slm_i);

    ui->combo_data->setEditable(true);
    ui->combo_data->lineEdit()->setReadOnly(true);
    ui->combo_data->lineEdit()->setAlignment(Qt::AlignCenter);
    for (unsigned i = 0; i < combo_data.size(); i++)
    {
        ui->combo_data->addItem(combo_data[i].c_str());
    }
    ui->combo_data->setCurrentIndex(combo_data_i);

    for (unsigned i = 0; i < combo_arch_to_nocw.size(); i++)
    {
        ui->combo_arch->addItem(get_nocw(i, 0).c_str());
    }

    ui->combo_arch->setEditable(true);
    ui->combo_arch->lineEdit()->setReadOnly(true);
    ui->combo_arch->lineEdit()->setAlignment(Qt::AlignCenter);
    // TODO: set the width depending on the chosen
    // processor (e.g., 32 leon3, 64 ariane, etc.)
    /* ui->lineEdit_arch->setText("leon3"); */
    ui->lineEdit_nocw->setText("32");
    // ui->lineEdit_nocw->setText("64");
    ui->lineEdit_espmac->setText(get_ESP_MAC().c_str());
    // ui->lineEdit_espip->setText(get_ESP_IP().c_str());

    /*
    ui->combo_l2_ways->addItem("2 ways");
    ui->combo_l2_ways->addItem("4 ways");
    ui->combo_l2_ways->addItem("8 ways");
    ui->combo_l2_ways->setCurrentIndex(1);
    */
    for (unsigned i = 0; i < l2_ways.size(); i++)
    {
        ui->combo_l2_ways->addItem(l2_ways[i].c_str());
    }
    ui->combo_l2_ways->setCurrentIndex(l2_ways_i);
    ui->combo_l2_ways->setEnabled(false);

    /*
    ui->combo_l2_sets->addItem("32 sets");
    ui->combo_l2_sets->addItem("64 sets");
    ui->combo_l2_sets->addItem("128 sets");
    ui->combo_l2_sets->addItem("256 sets");
    ui->combo_l2_sets->addItem("512 sets");
    ui->combo_l2_sets->addItem("1024 sets");
    ui->combo_l2_sets->addItem("2048 sets");
    ui->combo_l2_sets->addItem("4096 sets");
    ui->combo_l2_sets->setCurrentIndex(4);
    */
    for (unsigned i = 0; i < l2_sets.size(); i++)
    {
        ui->combo_l2_sets->addItem(l2_sets[i].c_str());
    }
    ui->combo_l2_sets->setCurrentIndex(l2_sets_i);
    ui->combo_l2_sets->setEnabled(false);

    /*
    ui->combo_llc_ways->addItem("4 ways");
    ui->combo_llc_ways->addItem("8 ways");
    ui->combo_llc_ways->addItem("16 ways");
    ui->combo_llc_ways->setCurrentIndex(2);
    */
    for (unsigned i = 0; i < llc_ways.size(); i++)
    {
        ui->combo_llc_ways->addItem(llc_ways[i].c_str());
    }
    ui->combo_llc_ways->setCurrentIndex(llc_ways_i);
    ui->combo_llc_ways->setEnabled(false);

    /*
    ui->combo_llc_sets->addItem("32 sets");
    ui->combo_llc_sets->addItem("64 sets");
    ui->combo_llc_sets->addItem("128 sets");
    ui->combo_llc_sets->addItem("256 sets");
    ui->combo_llc_sets->addItem("512 sets");
    ui->combo_llc_sets->addItem("1024 sets");
    ui->combo_llc_sets->addItem("2048 sets");
    ui->combo_llc_sets->addItem("4096 sets");
    ui->combo_llc_sets->setCurrentIndex(5);
    */
    for (unsigned i = 0; i < llc_sets.size(); i++)
    {
        ui->combo_llc_sets->addItem(llc_sets[i].c_str());
    }
    ui->combo_llc_sets->setCurrentIndex(llc_sets_i);
    ui->combo_llc_sets->setEnabled(false);

    /*
    ui->combo_al2_ways->addItem("2 ways");
    ui->combo_al2_ways->addItem("4 ways");
    ui->combo_al2_ways->addItem("8 ways");
    ui->combo_al2_ways->setCurrentIndex(1);
    */
    for (unsigned i = 0; i < al2_ways.size(); i++)
    {
        ui->combo_al2_ways->addItem(al2_ways[i].c_str());
    }
    ui->combo_al2_ways->setCurrentIndex(al2_ways_i);
    ui->combo_al2_ways->setEnabled(false);

    /*
    ui->combo_al2_sets->addItem("32 sets");
    ui->combo_al2_sets->addItem("64 sets");
    ui->combo_al2_sets->addItem("128 sets");
    ui->combo_al2_sets->addItem("256 sets");
    ui->combo_al2_sets->addItem("512 sets");
    ui->combo_al2_sets->addItem("1024 sets");
    ui->combo_al2_sets->addItem("2048 sets");
    ui->combo_al2_sets->addItem("4096 sets");
    ui->combo_al2_sets->setCurrentIndex(4);
    */
    for (unsigned i = 0; i < al2_sets.size(); i++)
    {
        ui->combo_al2_sets->addItem(al2_sets[i].c_str());
    }
    ui->combo_al2_sets->setCurrentIndex(al2_sets_i);
    ui->combo_al2_sets->setEnabled(false);

    /*
    ui->combo_implem->addItem("SystemVerilog");
    ui->combo_implem->addItem("SystemC (HLS)");
    */
    for (unsigned i = 0; i < implem.size(); i++)
    {
        ui->combo_implem->addItem(implem[i].c_str());
    }
    ui->combo_implem->setCurrentIndex(0);
    ui->combo_implem->setEnabled(false);

    ui->pushButton_gen->setEnabled(true);
}

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
// Destructor
//

espcreator::~espcreator()
{
    delete ui;
}

//
//
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
            /* printf("ethipm: %s\n", ethipm.c_str()); */
        }

        if (str.find("CFG_ETH_IPL") != std::string::npos)
        {
            ethipl = str.substr(str.find("16#") + 3, 4);
            /* printf("ethipl: %s\n", ethipl.c_str()); */
        }
    }

    char buf[20];
    str = ethipm + ethipl;
    int part1 = strtol(str.substr(0, 2).c_str(), NULL, 16);
    int part2 = strtol(str.substr(2, 2).c_str(), NULL, 16);
    int part3 = strtol(str.substr(4, 2).c_str(), NULL, 16);
    int part4 = strtol(str.substr(6, 2).c_str(), NULL, 16);
    sprintf(buf, "%d.%d.%d.%d", part1, part2, part3, part4);
    /* printf("ethipm: %s\n", buf); */
    return std::string(buf);
}

std::string espcreator::get_ESP_MAC()
{
    std::string str, ethipm, ethipl;
    std::ifstream ifs("grlib_config.vhd");
    while (std::getline(ifs, str))
    {
        if (str.find("CFG_ETH_ENM") != std::string::npos)
        {
            ethipm = str.substr(str.find("16#") + 3, 6);
            /* printf("ethipm: %s\n", ethipm.c_str()); */
        }

        if (str.find("CFG_ETH_ENL") != std::string::npos)
        {
            ethipl = str.substr(str.find("16#") + 3, 6);
            /* printf("ethipl: %s\n", ethipl.c_str()); */
        }
    }
    str = ethipm + ethipl;
    /*
    int length = str.length() + 3;
    for (int i = 2; i < length; i += 3)
        str.insert(i, ":");
    */
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    /* printf("ethipm: %s\n", str.c_str()); */
    return str;
}

std::string espcreator::get_MAC_Addr(std::string mac)
{
    int length = mac.length() + 3;
    for (int i = 2; i < length; i += 3)
        mac.insert(i, ":");
    std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
    return mac;
}

// added 06/30
std::string espcreator::get_nocw(int i, int j)
{
    return combo_arch_to_nocw[i][j];
}
// end added 06/30

//
// NoC Frame
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

    /* ui->pushButton_cfg->setEnabled(true); */

    NOCX = new_nocx;
    NOCY = new_nocy;
}

void espcreator::check_enable_noc_update()
{
    if (ui->spinBox_nocx->value() > 0 && ui->spinBox_nocy->value() > 0)
        ui->pushButton_noc->setEnabled(true);
    else
        ui->pushButton_noc->setEnabled(false);
}

void espcreator::on_spinBox_nocy_valueChanged(int arg1 __attribute__((unused)))
{
    check_enable_noc_update();
}

void espcreator::on_spinBox_nocx_valueChanged(int arg1 __attribute__((unused)))
{
    check_enable_noc_update();
}

void espcreator::on_spinBox_vf_valueChanged(int arg1 __attribute__((unused)))
{
    for (unsigned int y = 0; y < NOCY; y++)
        for (unsigned int x = 0; x < NOCX; x++)
            frame_tile[y][x]->set_vf_points_count(ui->spinBox_vf->value());
}

//
// Configuration checks
//
bool espcreator::check_present(tile_t type)
{
    FOREACH_TILE(y, x)
    if (frame_tile[y][x]->type == type)
        return true;
    return false;
}

bool espcreator::check_present(tile_t type, unsigned max_count)
{
    unsigned count = 0;
    for (int y = 0; y < (int)frame_tile.size(); y++)
        for (int x = 0; x < (int)frame_tile[y].size(); x++)
            if (frame_tile[y][x]->type == type)
                count++;
    if (count > 0 && count <= max_count)
        return true;
    else
        return false;
}

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
// Confiugration updates
//
void espcreator::update_power_info()
{
    REV_FOREACH_POW(i)
    {
        delete frame_power[i];
        frame_power.pop_back();
    }

    // Look for power domains different from zero
    FOREACH_TILE(y, x)
    {
        Power *pa = new Power(ui->frame_pow,
                              ui->layout_pow,
                              frame_power.size() + 1,
                              frame_tile[y][x]->id,
                              frame_tile[y][x]->ip,
                              frame_tile[y][x]->impl,
                              power_info_db);
        frame_power.push_back(pa);
    }
}

void espcreator::update_address_map()
{
    REV_FOREACH_ADDRESS(i)
    {
        ui->layout_addr->removeWidget(frame_address[i]);
        delete frame_address[i];
        frame_address.pop_back();
    }

    // Set accelerators starting address
    unsigned cpu_arch_bits = 32;
    unsigned long long accelerators_addr = 0;
    unsigned long long accelerators_mask = 0;

    if (cpu_arch == "leon3")
    {
        accelerators_addr = CFG_LEON3_AHB_APB_ADDR + CFG_LEON3_APB_ESP_ACCELERATORS_ADDR;
        accelerators_mask = CFG_LEON3_APB_ESP_ACCELERATORS_MASK;
        cpu_arch_bits = 32;
    } /* else if (cpu_arch == "zynq") {
         accelerators_addr = CFG_ZYNQ_PL_ADDR + CFG_ESP_ACCELERATOR_ADDR;
         cpu_arch_bits = 32;
         }*/

    // Count memory tiles
    unsigned mem_split = 1;
    FOREACH_TILE(y, x)
    if (frame_tile[y][x]->type == TILE_MEMDBG || frame_tile[y][x]->type == TILE_MEM)
        mem_split++;

    // Create address space
    FOREACH_TILE(y, x)
    {
        if (frame_tile[y][x]->type == TILE_ACC)
        {
            Address *fa = new Address(ui->layout_addr,
                                      frame_address.size() + 1,
                                      cpu_arch_bits,
                                      frame_tile[y][x]->id,
                                      frame_tile[y][x]->ip,
                                      accelerators_addr,
                                      accelerators_mask);
            frame_address.push_back(fa);
            accelerators_addr += frame_address.back()->size;
            connect(fa, SIGNAL(addressMapChanged()), this, SLOT(addressMapChanged()));
        }
        if (frame_tile[y][x]->type == TILE_MEMDBG)
        {
            Address *fa;
            fa = new Address(ui->layout_addr,
                             frame_address.size() + 1,
                             cpu_arch_bits,
                             frame_tile[y][x]->id,
                             to_string("L3 Debug Unit"),
                             CFG_LEON3_AHB_DSU3_ADDR,
                             CFG_LEON3_AHB_DSU3_MASK);
            frame_address.push_back(fa);
            connect(fa, SIGNAL(addressMapChanged()), this, SLOT(addressMapChanged()));
            fa = new Address(ui->layout_addr,
                             frame_address.size() + 1,
                             cpu_arch_bits,
                             frame_tile[y][x]->id,
                             to_string("GR Ethernet"),
                             CFG_LEON3_AHB_APB_ADDR + CFG_LEON3_APB_GRETH_ADDR,
                             CFG_LEON3_APB_GRETH_MASK);
            frame_address.push_back(fa);
            connect(fa, SIGNAL(addressMapChanged()), this, SLOT(addressMapChanged()));
            fa = new Address(ui->layout_addr,
                             frame_address.size() + 1,
                             cpu_arch_bits,
                             frame_tile[y][x]->id,
                             to_string("SGMII Adapter"),
                             CFG_LEON3_AHB_APB_ADDR + CFG_LEON3_APB_SGMII_ADDR,
                             CFG_LEON3_APB_SGMII_MASK);
            frame_address.push_back(fa);
            connect(fa, SIGNAL(addressMapChanged()), this, SLOT(addressMapChanged()));
        }
    }
    addressMapChanged();
}

QString espcreator::get_ok_bullet()
{
    QString ok_bullet = "<span style=\" color:#0000ff;\" > ";
    ok_bullet.append("<b># </b>");
    ok_bullet.append("</span>");
    ok_bullet.append("<span style=\" color:#000000;\" > ");
    return ok_bullet;
}

QString espcreator::get_err_bullet()
{
    QString err_bullet = "<span style=\" color:#ff0c32;\" > ";
    err_bullet.append("<b># </b>");
    err_bullet.append("</span>");
    err_bullet.append("<span style=\" color:#000000;\" > ");
    return err_bullet;
}
/*
void espcreator::on_combo_arch_currentIndexChanged(const QString &arg1)
{
    if (arg1.toStdString() == "leon3")
    {
        ui->lineEdit_nocw->setText("32");
    }
    else if (arg1.toStdString() == "ariane")
    {
        ui->lineEdit_nocw->setText("64");
    }
}
*/

// added 06/30
void espcreator::on_combo_arch_currentIndexChanged(const QString &arg1)
{
    for (unsigned i = 0; i < combo_arch_to_nocw.size(); i++)
    {
        if (arg1.toStdString() == combo_arch_to_nocw[i][0])
        {
            ui->lineEdit_nocw->setText(get_nocw(i, 1).c_str());
        }
    }
    // ui->lineEdit_nocw->setText(ui->lineEdit_mac->text()); for testing lineEdit_nocw
}
// end added 06/30

void espcreator::addressMapChanged()
{
    std::vector<QString> error_msg;
    QString err_bullet = get_err_bullet();
    QString error = err_bullet;
    error.append("Review and validate address map </span><br>");
    error_msg.push_back(error);
    ui->textBrowser_addr->setHtml(error);

    ui->pushButton_addr_confirm->setEnabled(false);
    ui->pushButton_gen->setEnabled(true);
    /* ui->pushButton_gen->setEnabled(true); */
    ui->pushButton_pow_confirm->setEnabled(false);
}

// void espcreator::on_pushButton_cfg_clicked()
// {
// 	std::vector<QString> error_msg;
// 	QString ok_bullet = get_ok_bullet();
// 	QString err_bullet = get_err_bullet();
//
// 	if (cpu_arch == "leon3") {
// 		if (!check_present(TILE_CPU, CFG_LEON3_NCPU_MAX)) {
// 			QString error = err_bullet;
// 			error.append(ECPUCOUNT(1)"</span><br>");
// 			error_msg.push_back(error);
// 		}
// 		if (!check_present(TILE_MEMDBG, 1)) {
// 			QString error = err_bullet;
// 			error.append(EMEMDBGCOUNT(1)"</span><br>");
// 			error_msg.push_back(error);
// 		}
// 		if (!check_present(TILE_MISC, 1)) {
// 			QString error = err_bullet;
// 			error.append(EMISCCOUNT(1)"</span><br>");
// 			error_msg.push_back(error);
// 		}
// 	}
//
// 	if (!check_clock_domains()) {
// 		QString error = err_bullet;
// 		error.append(ECLKDOMAINS"</span><br>");
// 		error_msg.push_back(error);
// 	}
//
// 	FOREACH_TILE(y, x)
// 		if (frame_tile[y][x]->type == TILE_ACC && frame_tile[y][x]->impl == "") {
// 			QString error = err_bullet;
// 			std::string s = EACCNOIMPL + (" " + to_string(frame_tile[y][x]->id)) +
// "</span><br>";
// 			error.append(s.c_str());
// 			error_msg.push_back(error);
// 		}
//
// 	if (error_msg.size() == 0) {
// 		ok_bullet.append("SoC configuration is valid.</span><br>");
// 		/* ui->textBrowser_msg->setHtml(ok_bullet); */
// 		/* ui->pushButton_gen->setEnabled(false); */
//         ui->pushButton_gen->setEnabled(true);
// 		ui->pushButton_pow_confirm->setEnabled(false);
// 		ui->tab_addr->setEnabled(true);
// 		ui->pushButton_addr_reset->setEnabled(true);
// 		ui->pushButton_addr_validate->setEnabled(true);
// 		update_address_map();
// 		ui->tab_pow->setEnabled(true);
// 		update_power_info();
// 		ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tab_addr));
// 		ui->tabWidget->setCurrentWidget(ui->tab_addr);
// 	} else {
// 		QString err = "";
// 		for (int i = 0; i < (int) error_msg.size(); i++)
// 			err.append(error_msg[i]);
// 		/* ui->textBrowser_msg->setHtml(err); */
// 		ui->pushButton_gen->setEnabled(true);
// 		/* ui->pushButton_gen->setEnabled(true); */
// 		ui->pushButton_pow_confirm->setEnabled(false);
// 		ui->tab_addr->setEnabled(false);
// 		ui->tab_pow->setEnabled(false);
// 	}
// }

void espcreator::on_pushButton_addr_reset_clicked()
{
    update_address_map();
}

void espcreator::on_pushButton_addr_validate_clicked()
{
    bool address_map_is_valid = true;
    FOREACH_ADDRESS(i)
    frame_address[i]->set_conflicting(false);
    FOREACH_ADDRESS_PAIR(i, j)
    {
        if (frame_address[i]->base > frame_address[j]->end ||
            frame_address[i]->end < frame_address[j]->base)
        {
            continue;
        }
        else
        {
            address_map_is_valid = false;
            frame_address[i]->set_conflicting(true);
            frame_address[j]->set_conflicting(true);
        }
    }
    if (address_map_is_valid)
    {
        QString ok_bullet = get_ok_bullet();
        QString ok = ok_bullet;
        ok.append("Address map is valid </span><br>");
        ui->textBrowser_addr->setHtml(ok);
        ui->pushButton_addr_confirm->setEnabled(true);
    }
}

void espcreator::on_pushButton_addr_confirm_clicked()
{
    ui->pushButton_pow_confirm->setEnabled(true);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tab_pow));
    ui->tabWidget->setCurrentWidget(ui->tab_pow);
}

void espcreator::on_pushButton_pow_confirm_clicked()
{
    ui->pushButton_gen->setEnabled(true);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tab_soc));
    ui->tabWidget->setCurrentWidget(ui->tab_soc);
}

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
    if (tot_mem > NMEM_MAX)
    {
        gen_ok = false;
        errors_s += "There must be no more than " + to_string(NMEM_MAX) + "\n";
    }

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

    if (errors_s.length() > 0)
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
            std::cout << "Save\n";
            break;
        case QMessageBox::Cancel:
            std::cout << "Cancel\n";
            return;
        default:
            break;
    }


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

    /*
    // If configuration file exists, then backup
    int source = open(cfg_file_name.c_str(), O_RDONLY, 0);
    if (source)
    {
        int dest = open(bkp_file_name.c_str(), O_WRONLY | O_CREAT, 0644);
        struct stat stat_source;
        fstat(source, &stat_source);
        sendfile(dest, source, 0, stat_source.st_size);
        ::close(source);
        ::close(dest);
    }
    */

    // std::ofstream cfg(cfg_file_name.c_str()); // removed

    // Write new configuration
    std::cerr << bkp_file_name.c_str() << "\n";
    std::ofstream fp(bkp_file_name.c_str());

    // CPU_ARCH
    fp << "CPU_ARCH = " << cpu_arch_s.c_str() << "\n";

    // NCPU_TILE
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
    if (cache_en)
        fp << "CONFIG_CACHE_EN = y\n";
    else
        fp << "#CONFIG_CACHE_EN is not set\n";

    // CONFIG_CACHE_RTL TODO: FIX
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
    fp << "CONFIG_SLM_KBYTES = " << slm_s.c_str() << "\n";

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
            if (frame_tile[y][x]->get_type() == "acc")
                fp << " " << frame_tile[y][x]->get_impl_acc().c_str() << " " << frame_tile[y][x]->get_acc_l2() << " sld"; // change get_impl() to some other function
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
    system(sys.c_str());

    QMessageBox msgBox_saved;
    QString saved_msg = "New configuration file saved in " + QString::fromUtf8(bkp_file_name.c_str());
    msgBox_saved.setText(saved_msg);
    msgBox_saved.exec();
}
