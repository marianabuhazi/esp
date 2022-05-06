#include "tile.h"
#include "socmap_utils.h"
#include "constants.h"

/* ------------------------------------------------------------------------- */
/* -------------------------------- Classes -------------------------------- */
/* ------------------------------------------------------------------------- */

//
// Constructor
//
Tile::Tile(QWidget *parent,
           QGridLayout *parent_layout,
           unsigned y,
           unsigned x,
           std::string cpu_arch)
    : QFrame(parent)
{
    // Set known variables
    nocy = y;
    nocx = x;
    this->popup_active = false;

    // Create object name
    std::string name("tile_" + to_string(y) + "_" + to_string(x));
    this->setObjectName(QString::fromUtf8(name.c_str()));

    // Set size policy
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    /* sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth()); */
    this->setFixedWidth(237);
    this->setFixedHeight(160);
    this->setSizePolicy(sizePolicy);

    // Set appearance
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);

    // Prepare grid for configuration objects
    layout = new QGridLayout(this);
    layout->setSpacing(2);
    layout->setContentsMargins(7, 7, 7, 7);

    // Tile ID
    id_l = new QLabel("Tile ");
    layout->addWidget(id_l, 0, 0, 1, 1);

    // Type selector
    type_sel = new QComboBox(this);
    type_sel->setObjectName(QString::fromUtf8("type_sel"));
    type_sel->clear();
    type_sel->addItem(tile_t_to_string(TILE_EMPTY).c_str());
    type_sel->setItemData(0, tile_t_to_string(TILE_EMPTY).c_str(), Qt::ToolTipRole);
    type_sel->addItem(tile_t_to_string(TILE_ACC).c_str());
    type_sel->setItemData(1, tile_t_to_string(TILE_ACC).c_str(), Qt::ToolTipRole);

    this->cpu_arch = cpu_arch;
    type_sel->addItem(tile_t_to_string(TILE_CPU).c_str());
    type_sel->setItemData(2, tile_t_to_string(TILE_CPU).c_str(), Qt::ToolTipRole);
    type_sel->addItem(tile_t_to_string(TILE_MEM).c_str());
    type_sel->setItemData(3, tile_t_to_string(TILE_MEM).c_str(), Qt::ToolTipRole);
    type_sel->addItem(tile_t_to_string(TILE_MISC).c_str());
    type_sel->setItemData(4, tile_t_to_string(TILE_MISC).c_str(), Qt::ToolTipRole);
    type_sel->addItem(tile_t_to_string(TILE_SLM).c_str());
    type_sel->setItemData(5, tile_t_to_string(TILE_SLM).c_str(), Qt::ToolTipRole);
    type_sel->setToolTip(type_sel->currentText());
    type = TILE_EMPTY;
    layout->addWidget(type_sel, 0, 1, 1, 3);

    // IP selector
    ip_sel_l = new QLabel("IP: ", this);
    ip_sel_l->setAlignment(Qt::AlignRight);
    QSizePolicy ip_sel_l_sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ip_sel_l_sizePolicy.setHorizontalStretch(0);
    ip_sel_l_sizePolicy.setVerticalStretch(0);
    ip_sel_l_sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    ip_sel_l->setSizePolicy(ip_sel_l_sizePolicy);
    layout->addWidget(ip_sel_l, 1, 0, 1, 1);
    ip_sel = new QComboBox(this);
    ip_sel->setObjectName(QString::fromUtf8("ip_sel"));
    ip_sel->clear();
    ip_sel->addItem("");
    ip_sel->setToolTip(ip_sel->currentText());
    ip_sel->setMinimumWidth(90);
    third_party_acc = false;


    // Power Information button
    power_popup = new QPushButton("Power Information", this);
    layout->addWidget(power_popup, 5, 0, 1, 5);
    power_popup->setEnabled(false);
    connect(power_popup, &QPushButton::released, this, &Tile::on_power_popup);

    get_list(CONSTANT_IP_PATH, ip_sel);
    get_list(CONSTANT_THIRD_PARTY_PATH, ip_sel);

    ip_sel->setEnabled(false);
    ip = "empty";
    layout->addWidget(ip_sel, 1, 1, 1, 2);

    // IP implementation selector
    impl_sel_l = new QLabel("Impl.: ", this);
    impl_sel_l->setAlignment(Qt::AlignRight);
    QSizePolicy impl_sel_l_sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    impl_sel_l_sizePolicy.setHorizontalStretch(0);
    impl_sel_l_sizePolicy.setVerticalStretch(0);
    impl_sel_l_sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    impl_sel_l->setSizePolicy(impl_sel_l_sizePolicy);
    layout->addWidget(impl_sel_l, 2, 0, 1, 1);
    impl_sel = new QComboBox(this);
    impl_sel->setObjectName(QString::fromUtf8("ip_sel"));
    impl_sel->clear();
    impl_sel->addItem("");
    // TODO: We need to use the HLS configurations like in the old gui.
    // These are hard-coded just for testing but should be removed.
    impl_sel->addItem("dma32");
    impl_sel->addItem("dma64");
    impl_sel->setToolTip(impl_sel->currentText());
    impl_sel->setEnabled(false);
    impl = "none";
    layout->addWidget(impl_sel, 2, 1, 1, 2);

    has_caches = new QCheckBox("ACC L2", this);
    has_caches->setChecked(false);
    has_caches->setObjectName(QString::fromUtf8("has_caches"));
    has_caches->setToolTip("Enable ACC L2 Caches");
    has_caches->setEnabled(false);
    layout->addWidget(has_caches, 4, 3, 1, 2);

    ip_sel_l->setStyleSheet(color_gray);
    impl_sel_l->setStyleSheet(color_gray);
    has_caches->setStyleSheet(color_gray);

    // Domain selector
    domain_sel_l = new QLabel("Clock domain:", this);
    domain_sel_l->setAlignment(Qt::AlignRight);
    QSizePolicy domain_sel_l_sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    domain_sel_l_sizePolicy.setHorizontalStretch(0);
    domain_sel_l_sizePolicy.setVerticalStretch(0);
    domain_sel_l_sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    domain_sel_l->setSizePolicy(domain_sel_l_sizePolicy);
    layout->addWidget(domain_sel_l, 4, 0, 1, 2);
    domain_sel = new QSpinBox(this);
    domain_sel->setMinimum(0);
    domain_sel->setObjectName(QString::fromUtf8("domain_sel"));
    domain_sel->setEnabled(false);
    domain = 0;
    layout->addWidget(domain_sel, 4, 2, 1, 1);

    // Has PLL
    has_pll_sel = new QCheckBox("PLL", this);
    has_pll_sel->setChecked(false);
    has_pll_sel->setObjectName(QString::fromUtf8("has_pll_sel"));
    has_pll_sel->setToolTip("Has PLL");
    has_pll_sel->setEnabled(false);
    has_pll = false;
    layout->addWidget(has_pll_sel, 1, 3, 1, 2);

    // Extra clock buffer
    extra_buf_sel = new QCheckBox("Clock buf.", this);
    extra_buf_sel->setChecked(false);
    extra_buf_sel->setObjectName(QString::fromUtf8("extra_buf_sel"));
    extra_buf_sel->setToolTip("Add extra clock buffer");
    extra_buf_sel->setEnabled(false);
    extra_buf = false;
    layout->addWidget(extra_buf_sel, 2, 3, 1, 2);

    // Has cache
    has_cache_sel = new QCheckBox("Has cache", this);
    has_cache_sel->setChecked(false);
    has_cache_sel->setObjectName(QString::fromUtf8("has_cache_sel"));
    has_cache_sel->setToolTip("Has cache");
    has_cache_sel->setEnabled(false);
    has_cache = false;
    layout->addWidget(has_cache_sel, 3, 1, 1, 2);

    // Has DDR
    has_ddr_sel = new QCheckBox("Has DDR", this);
    has_ddr_sel->setChecked(false);
    has_ddr_sel->setObjectName(QString::fromUtf8("has_ddr_sel"));
    has_ddr_sel->setToolTip("Has DDR");
    has_ddr_sel->setEnabled(false);
    has_ddr = false;
    layout->addWidget(has_ddr_sel, 3, 3, 1, 2);

    // Background color
    socmap::set_background_color(this, color_empty);

    // Connect signals to slot
    connect(this->type_sel,
            SIGNAL(currentIndexChanged(const QString &)),
            this,
            SLOT(on_type_sel_currentIndexChanged(const QString &)));
    connect(this->ip_sel,
            SIGNAL(currentIndexChanged(const QString &)),
            this,
            SLOT(on_ip_sel_currentIndexChanged(const QString &)));
    connect(this->impl_sel,
            SIGNAL(currentIndexChanged(const QString &)),
            this,
            SLOT(on_impl_sel_currentIndexChanged(const QString &)));
    connect(this->domain_sel,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(on_domain_sel_valueChanged(int)));
    connect(this->has_pll_sel,
            SIGNAL(toggled(bool)),
            this,
            SLOT(on_has_pll_sel_toggled(bool)));
    connect(this->extra_buf_sel,
            SIGNAL(toggled(bool)),
            this,
            SLOT(on_extra_buf_sel_toggled(bool)));

    // Add tile frame to parent frame
    parent_layout->addWidget(this, y, x, 1, 1);
}

//
// Destructor
//
Tile::~Tile()
{
    ip_list.clear();
    ip_list_3.clear();
    impl_list.clear();
    delete id_l;
    delete type_sel;
    delete ip_sel_l;
    delete ip_sel;
    delete impl_sel_l;
    delete impl_sel;
    delete domain_sel_l;
    delete domain_sel;
    delete has_pll_sel;
    delete extra_buf_sel;
    delete layout;
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- Get Functions ----------------------------- */
/* ------------------------------------------------------------------------- */

//
// get type
//
std::string Tile::get_type()
{
    QString get_type_q = type_sel->currentText();
    std::string get_type_s = get_type_q.toUtf8().constData();
    for (unsigned int i = 0; i < tile_type.size(); i++)
    {
        if (get_type_s == tile_type[i][0])
        {
            return tile_type[i][1];
        }
    }
    return "n/a";
}

//
// get ip
//
std::string Tile::get_ip()
{
    return ip;
}

//
// get impl
//
std::string Tile::get_impl()
{
    QString get_impl_q = impl_sel->currentText();
    std::string get_impl_s = get_impl_q.toUtf8().constData();
    return get_impl_s;
}

//
// get ip acc
//
std::string Tile::get_ip_acc()
{
    std::string ip_cap = ip;
    transform(ip_cap.begin(), ip_cap.end(),ip_cap.begin(), ::toupper);
    return ip_cap;
}

//
// get impl acc
//
std::string Tile::get_impl_acc()
{
    QString get_impl_q = impl_sel->currentText();
    std::string get_impl_s = get_impl_q.toUtf8().constData();

    std::string get_impl_s_sub = get_impl_s.substr(ip.length() + 1, get_impl_s.length() - ip.length() - 1);
    return get_impl_s_sub;
}

//
// get impl acc 3
//
std::string Tile::get_impl_acc_3()
{
    QString get_impl_q = impl_sel->currentText();
    std::string get_impl_s = get_impl_q.toUtf8().constData();
    return get_impl_s;
}

//
// get domain
//
std::string Tile::get_domain()
{
    return to_string(domain_sel->value());
}

//
// get PLL
//
std::string Tile::get_PLL()
{
    return to_string(has_pll_sel->isChecked());
}

//
// get buf
//
std::string Tile::get_buf()
{
    return to_string(extra_buf_sel->isChecked());
}

//
// get power
//
std::string Tile::get_power()
{
    std::string tile_power;
    for (int i = 0; i < 3 * vf_points_count; i++)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << vf_points[i];
        std::string tmp = stream.str();
        tile_power += tmp + " ";
    }
    return tile_power;
}

//
// get acc l2
//
std::string Tile::get_acc_l2()
{
    return to_string((int)has_caches->isChecked());
}

//
// get has ddr sel
//
std::string Tile::get_has_ddr_sel()
{
    return to_string((int)has_ddr_sel->isChecked());
}

//
// get vendor
//
std::string Tile::get_vendor()
{
    if (third_party_acc)
    {
        std::string vendor_path(CONSTANT_THIRD_PARTY_PATH + ip + "/vendor");
        std::ifstream infile(vendor_path);
        std::string sLine;
        std::getline(infile, sLine);
        return sLine;
    } else {
        return "sld";
    }
}

//
// get third party acc
//
bool Tile::get_third_party_acc()
{
    return third_party_acc;
}

//
// get list
//
void Tile::get_list(const std::string path, QComboBox *sel)
{
    std::vector<std::string> list;

    get_subdirs(path, list);
    for (unsigned i = 0; i < list.size(); i++) {
        sel->addItem(list[i].c_str());
        sel->setItemData(i + 1, list[i].c_str(), Qt::ToolTipRole);
    }
}

//
// get third party impl list
//
void Tile::update_third_party_impl_list(std::string ip, QComboBox *impl_sel)
{
    for (unsigned i = 0; i < third_party_impl.size(); i++)
    {
        if (ip == third_party_impl[i][0][0]) {
            for (unsigned j = 0; j < third_party_impl[i][1].size(); j++)
            {
                impl_sel->addItem(third_party_impl[i][1][j].c_str());
                impl_sel->setItemData(j + 1, third_party_impl[i][1][j].c_str(), Qt::ToolTipRole);
            }
        }
    }
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- Set Functions ----------------------------- */
/* ------------------------------------------------------------------------- */

//
// set id
//
void Tile::set_id(unsigned id)
{
    std::string s("Tile " + to_string(id) + ":");
    id_l->setText(s.c_str());
    this->id = id;
}

//
// set type
//
void Tile::set_type(std::string type)
{
    std::string type_abbrev = "Accelerator";
    for (unsigned int i = 0; i < tile_type.size(); i++)
    {
        if (type == tile_type[i][1])
        {
            type_abbrev = tile_type[i][0];
        }
    }
    if (type == "IO")
        type_abbrev = "Miscellaneous";
    type_sel->setCurrentText(QString::fromStdString(type_abbrev));
}

//
// set ip
//
void Tile::set_ip(std::string ip)
{
    ip_sel->setCurrentText(QString::fromStdString(ip));
    std::transform(ip.begin(), ip.end(), ip.begin(), [](unsigned char c){ return std::tolower(c); });
    ip_sel->setCurrentText(QString::fromStdString(ip));
}

//
// set impl
//
void Tile::set_impl(std::string impl)
{
    std::transform(impl.begin(), impl.end(), impl.begin(), [](unsigned char c){ return std::tolower(c); });
    impl_sel->setCurrentText(QString::fromStdString(impl));
}

//
// set domain
//
void Tile::set_domain(std::string domain)
{
    domain_sel->setValue(stoi(domain));
}

//
// set PLL
//
void Tile::set_pll(std::string pll)
{
    has_pll_sel->setChecked(stoi(pll));
    has_pll = stoi(pll);
}

//
// set buf
//
void Tile::set_buf(std::string buf)
{
    extra_buf_sel->setChecked(stoi(buf));
    extra_buf = stoi(buf);
}

//
// set cache
//
void Tile::set_cache(std::string cache)
{
    has_cache_sel->setChecked(stoi(cache));
}

//
// set acc l2
//
void Tile::set_acc_l2(std::string value)
{
    has_caches->setChecked(stoi(value));
}

//
// set vf points
//
void Tile::set_vf_points_count(int vf)
{
    this->vf_points_count = vf;
    if ((int)vf_points.size() < 3 * vf)
    {
        vf_points.resize(3 * vf, 0);
    }
}

//
// set vf spin boxes
//
void Tile::set_vf_spin_boxes(int index, std::string value)
{
    vf_points[index] = stof(value);
}

/* ------------------------------------------------------------------------- */
/* --------------------------------- Slots --------------------------------- */
/* ------------------------------------------------------------------------- */

//
// On type changed
//
void Tile::on_type_sel_currentIndexChanged(const QString &arg1)
{
    tile_reset();
    domain_setEnabled(false);

    if (arg1 != tile_t_to_string(TILE_ACC).c_str())
    {

        ip_sel_l->setStyleSheet(color_gray);
        impl_sel_l->setStyleSheet(color_gray);
        has_caches->setStyleSheet(color_gray);
        has_caches->setEnabled(false);
    }
    else
    {
        ip_sel_l->setStyleSheet(color_black);
        impl_sel_l->setStyleSheet(color_black);
        has_caches->setStyleSheet(color_black);
        has_caches->setEnabled(true);
    }

    if (arg1 == tile_t_to_string(TILE_EMPTY).c_str())
    {
        type = TILE_EMPTY;
        ip = "empty";
        impl = "none";
        set_background_color(this, color_empty);
    }
    else if (arg1 == tile_t_to_string(TILE_ACC).c_str())
    {
        type = TILE_ACC;
        ip = "acc";
        set_background_color(this, color_acc);
        ip_sel->setEnabled(true);
        domain_setEnabled(true);
        has_caches->setEnabled(true);
    }
    else if (arg1 == tile_t_to_string(TILE_AXI).c_str())
    {
        type = TILE_AXI;
        ip = "axi";
        impl = "default";
        set_background_color(this, color_axi);
    }
    else if (arg1 == tile_t_to_string(TILE_CPU).c_str())
    {
        type = TILE_CPU;
        ip = "cpu";
        impl = "rtl";
        socmap::set_background_color(this, color_cpu);
        domain_setEnabled(true);
        has_caches->setEnabled(false);
    }
    else if (arg1 == tile_t_to_string(TILE_MEM).c_str())
    {
        type = TILE_MEM;
        ip = "mem";
        impl = "rtl";
        set_background_color(this, color_mem);
        has_caches->setEnabled(false);
    }
    else if (arg1 == tile_t_to_string(TILE_MISC).c_str())
    {
        type = TILE_MISC;
        ip = "misc";
        impl = "rtl";
        set_background_color(this, color_misc);
        has_caches->setEnabled(false);
    }
    else if (arg1 == tile_t_to_string(TILE_SLM).c_str())
    {
        type = TILE_SLM;
        ip = "slm";
        impl = "rtl";
        set_background_color(this, color_slm);
        has_caches->setEnabled(false);
    }
}

//
// On ip changed
//
void Tile::on_ip_sel_currentIndexChanged(const QString &arg1)
{
    impl_reset();

    QString test_q = arg1;
    std::string test_s = test_q.toUtf8().constData();
    ip = ip_sel->currentText().toStdString();
    ip_sel->setToolTip(ip_sel->currentText());
    impl_sel->setToolTip(impl_sel->currentText());
    if (arg1 != "")
    {
        third_party_acc = false;
        for (unsigned i = 0; i < ip_list_3.size(); i++)
        {
            if (arg1.toStdString() == ip_list_3[i])
                third_party_acc = true;
        }
        std::string impl_path(CONSTANT_IP_PATH + arg1.toStdString());
        get_list(impl_path, impl_sel);
        update_third_party_impl_list(ip, impl_sel);
        impl_sel->setEnabled(true);
    }
}

//
// On impl changed
//
void Tile::on_impl_sel_currentIndexChanged(const QString &arg1 __attribute__((unused)))
{
    impl_sel->setCurrentText(arg1);
    impl = impl_sel->currentText().toStdString();
    impl_sel->setToolTip(impl_sel->currentText());
}

//
// On domain changed
//
void Tile::on_domain_sel_valueChanged(int arg1)
{
    domain = arg1;
    if (arg1 == 0)
    {
        clocking_reset();
        clocking_setEnabled(false);
    }
    else
    {
        clocking_setEnabled(true);
    }
}

//
// On PLL changed
//
void Tile::on_has_pll_sel_toggled(bool arg1)
{
    if (!arg1)
        clocking_reset();
    else
        has_pll = true;
}

//
// On extra buf toggled
//
void Tile::on_extra_buf_sel_toggled(bool arg1)
{
    extra_buf = arg1;
}

//
// On cache toggled
//
void Tile::on_has_cache_sel_toggled(bool arg1)
{
    has_cache = arg1;
}

//
// On ddr toggled
//
void Tile::on_has_ddr_sel_toggled(bool arg1)
{
    has_ddr = arg1;
}

//
// On power popup
//
void Tile::on_power_popup()
{
    if (this->popup_active)
        return;

    this->popup_active = true;
    QDialog *popup = new QDialog;
    std::string popup_title("Power Information: Tile " + to_string(this->id));
    popup->setWindowTitle(QString::fromUtf8(popup_title.c_str()));
    QGridLayout *gridLayout = new QGridLayout(popup);

    // Column labels
    QLabel *i_l = new QLabel(tr("i"));
    gridLayout->addWidget(i_l, 0, 0);
    QLabel *voltage_l = new QLabel(tr("Voltage (V)"));
    gridLayout->addWidget(voltage_l, 0, 1);
    QLabel *frequency_l = new QLabel(tr("Frequency (MHz)"));
    gridLayout->addWidget(frequency_l, 0, 2);
    QLabel *tot_energy_l = new QLabel(tr("Tot Energy (pJ)"));
    gridLayout->addWidget(tot_energy_l, 0, 3);

    QVector<QDoubleSpinBox*> spin_boxes;
    int next_row = 0;
    for (int v = 0; v < this->vf_points_count; v++)
    {
        gridLayout->addWidget(new QLabel(tr(to_string(v).c_str())), v + 1, 0);

        spin_boxes.append(new QDoubleSpinBox(popup));
        spin_boxes[3 * v]->setMinimum(0);
        spin_boxes[3 * v]->setDecimals(1);
        spin_boxes[3 * v]->setSingleStep(0.1);
        spin_boxes[3 * v]->setValue(vf_points[3 * v]);
        spin_boxes.append(new QDoubleSpinBox(popup));
        spin_boxes[3 * v + 1]->setMinimum(0);
        spin_boxes[3 * v + 1]->setDecimals(1);
        spin_boxes[3 * v + 1]->setSingleStep(0.1);
        spin_boxes[3 * v + 1]->setValue(vf_points[3 * v + 1]);
        spin_boxes.append(new QDoubleSpinBox(popup));
        spin_boxes[3 * v + 2]->setMinimum(0);
        spin_boxes[3 * v + 2]->setDecimals(1);
        spin_boxes[3 * v + 2]->setSingleStep(0.1);
        spin_boxes[3 * v + 2]->setValue(vf_points[3 * v + 2]);

        gridLayout->addWidget(spin_boxes[3 * v], v + 1, 1);
        gridLayout->addWidget(spin_boxes[3 * v + 1], v + 1, 2);
        gridLayout->addWidget(spin_boxes[3 * v + 2], v + 1, 3);
        next_row = v + 1;
    }
    // buttons (save, cancel)
    QPushButton *save_b = new QPushButton("&Save", popup);
    gridLayout->addWidget(save_b, next_row + 1, 1);
    connect(save_b, &QPushButton::released, popup, &QDialog::accept);
    QPushButton *cancel_b = new QPushButton("&Cancel", popup);
    gridLayout->addWidget(cancel_b, next_row + 1, 2);
    connect(cancel_b, &QPushButton::released, popup, &QDialog::reject);

    int ret = popup->exec();
    this->popup_active = false;

    if (ret == 1) // Save
    {
        for (int i = 0; i < 3 * vf_points_count; i++)
        {
            vf_points[i] = spin_boxes[i]->value();
        }
    }
}

/* ------------------------------------------------------------------------- */
/* ------------------------------- Functions ------------------------------- */
/* ------------------------------------------------------------------------- */

//
// Reset tile
//
void Tile::tile_reset()
{
    type_sel->setToolTip(type_sel->currentText());
    ip_sel->setCurrentIndex(0);
    ip_sel->setEnabled(false);
    ip = "empty";
    ip_sel->setToolTip(ip_sel->currentText());
    impl_reset();
    domain_reset();
}

//
// Reset impl
//
void Tile::impl_reset()
{
    impl_sel->clear();
    impl_sel->addItem("");
    impl_sel->setCurrentIndex(0);
    impl_sel->setEnabled(false);
    impl_sel->setToolTip(impl_sel->currentText());
    impl_list.clear();
    impl = "";
}

//
// Reset domain
//
void Tile::domain_reset()
{
    domain_sel->setValue(0);
    domain = 0;
    clocking_reset();
}

//
// Reset clocking
//
void Tile::clocking_reset()
{
    has_pll_sel->setChecked(false);
    has_pll = false;
    extra_buf_sel->setChecked(false);
    extra_buf = false;
}

//
// On enabled domain
//
void Tile::domain_setEnabled(bool en)
{
    domain_sel->setEnabled(en);
    if (!en)
        clocking_setEnabled(en);
}

//
// On enabled clocking
//
void Tile::clocking_setEnabled(bool en)
{
    has_pll_sel->setEnabled(en);
    extra_buf_sel->setEnabled(en);
    has_cache_sel->setEnabled(en);
    has_ddr_sel->setEnabled(en);
    power_popup->setEnabled(en);
}

