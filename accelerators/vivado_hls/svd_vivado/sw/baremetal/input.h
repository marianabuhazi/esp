#define Q 177
#define P 229
#define M 3

float inputX[] = {
    -0.99729546, -0.48864438, -1.1597335,  -1.83951924, -0.50367543, -0.67540379, -1.76116705,
    -0.56847006, -0.96412516, -1.54168092, -0.5463881,  -1.02347615, -1.03408063, -0.43882483,
    -0.9527715,  -0.18104134, -0.31183941, -1.03191413, 0.03380437,  -0.22155062, -0.83721433,
    0.27832987,  -0.15683648, -0.75567541, 0.59277529,  -0.08146049, -0.67969061, 0.67739108,
    -0.09200194, -0.77271307, -1.16252282, -0.37445975, -0.63353585, -1.49612904, -0.35916294,
    -0.36346594, -1.06379629, -0.24854783, -0.23329892, -0.79043955, -0.1960456,  -0.21518729,
    -0.30572942, -0.12891756, -0.27866246, 0.29249919,  -0.15121653, -0.7440668,  0.26642661,
    -0.28253724, -1.21076763, 0.25278369,  -0.28297691, -1.20364431, 0.70436363,  -0.17895356,
    -1.11007509, -0.86198428, -0.46447789, -1.15747542, -1.23597398, -0.40344678, -0.69317616,
    -0.60140668, -0.23133928, -0.46623161, -0.34448123, -0.2839696,  -0.82459681, -0.15019629,
    -0.18288435, -0.57697954, 0.30655048,  -0.10973043, -0.60035504, 0.29631606,  -0.20083873,
    -0.92917766, 0.47144867,  -0.16347318, -0.9038481,  -1.08796523, -0.50364688, -1.15686221,
    -1.50668164, -0.44147577, -0.65970751, -1.309915,   -0.29544472, -0.2482296,  -0.53380432,
    -0.13703874, -0.16241689, -0.11302521, -0.13920615, -0.44001255, 0.18429409,  -0.25867852,
    -1.07031387, 0.02627099,  -0.24613202, -0.92287431, -0.2038614,  -0.14037788, -0.38612191,
    0.21845463,  -0.12557874, -0.60224647, 0.55245048,  -0.06992128, -0.611375,   0.46761183,
    -0.12078196, -0.74423818, -0.85144355, -0.34795755, -0.73530435, -1.21139075, -0.31881554,
    -0.39739047, -1.29475649, -0.48073934, -0.94003389, -0.80999666, -0.35880384, -0.80178821,
    -0.04373048, -0.20582957, -0.72966229, 0.29414384,  -0.28194672, -1.22635384, 0.37858178,
    -0.22287726, -1.06301641, 0.62831113,  -0.07339857, -0.67278355, -0.53571822, -0.30416314,
    -0.77639516, -0.75683557, -0.33864606, -0.76164837, -1.20502338, -0.35188437, -0.52320066,
    -0.98234606, -0.24068659, -0.2565506,  -0.5756377,  -0.13006537, -0.10994208, -0.24432534,
    -0.07424376, -0.11674686, 0.39516666,  -0.05329524, -0.44939188, 0.56069302,  -0.05207638,
    -0.5509673,  0.40109488,  -0.09471798, -0.6056724,  -1.23352538, -0.36711468, -0.56100238,
    -1.02413994, -0.32713367, -0.54799276, -0.53015517, -0.19121724, -0.36419256, -0.53347852,
    -0.23002649, -0.50492451, -0.23135237, -0.07129729, -0.11421308, 0.44309027,  0.11597712,
    0.14301224,  0.67889422,  0.07747944,  -0.14979527, 0.88347984,  -0.01951004, -0.63791468,
    0.62550769,  -0.09420377, -0.74757365, -1.2588954,  -0.40281823, -0.67617535, -1.71322933,
    -0.52775981, -0.84498236, -1.10442115, -0.30808598, -0.42643522, -0.71132108, -0.19862481,
    -0.27537737, -0.56847941, -0.27290335, -0.64033223, 0.02101895,  -0.19769009, -0.74118858,
    0.2476203,   -0.2111787,  -0.93603814, 0.17962703,  -0.24795267, -1.0278403,  0.45068449,
    -0.09193783, -0.62721324, -0.68375042, -0.25756313, -0.51000238, -1.25386173, -0.31554894,
    -0.35815215, -0.70146685, -0.17462712, -0.19335322, -0.02481276, -0.11360631, -0.40229926,
    -0.03260217, -0.30731293, -1.11036505, 0.46540757,  -0.23182222, -1.15157811, 0.52189233,
    -0.14884359, -0.88231697, 0.42812286,  -0.1501772,  -0.82714272, -0.88803573, -0.43858895,
    -1.04548253, -1.03458071, -0.42909264, -0.91662573, -1.44278041, -0.52164182, -0.99575339,
    -1.32433393, -0.67916794, -1.651521,   -0.5204829,  -0.51670091, -1.56853391, 0.08591836,
    -0.27641591, -1.07257236, 0.45036525,  -0.14954924, -0.83908308, 0.48592627,  -0.10825352,
    -0.70985465, 0.57771656,  -0.11552276, -0.79542884, -0.89866988, -0.3338689,  -0.65318177,
    -1.11515823, -0.34938933, -0.57159786, -0.63984076, -0.29961082, -0.69292031, -0.39300624,
    -0.26851156, -0.73660121, -0.00770975, -0.13878591, -0.50594722, 0.43843887,  -0.05446657,
    -0.48143064, 0.60952159,  -0.10765032, -0.78682876, 0.76971446,  -0.09912557, -0.85809287,
    -1.08566376, -0.3661673,  -0.65225834, -0.85363245, -0.31975781, -0.6300952,  -0.05384969,
    -0.08173553, -0.26637357, 0.37180037,  0.01359074,  -0.18820457, 0.49558317,  -0.11011876,
    -0.72290853, 0.69295672,  -0.09480981, -0.79302297, -1.14052985, -0.36609861, -0.61684965,
    -1.42400969, -0.3725923,  -0.459112,   -1.07432154, -0.36479113, -0.65446011, -0.75614393,
    -0.41475978, -1.04227511, -0.08972033, -0.35309519, -1.24229608, 0.34921658,  -0.2380767,
    -1.10015137, 0.46302236,  -0.27384477, -1.30473972, 0.76750977,  -0.13688358, -0.99567187,
    0.76668438,  -0.06811382, -0.74199342, -0.53640514, -0.34539402, -0.92773074, -0.51516409,
    -0.24674081, -0.57818702, -0.57586279, -0.26573382, -0.60920939, -0.31980777, -0.27111508,
    -0.79308752, 0.08685978,  -0.14442632, -0.58730633, 0.33341,     -0.05170875, -0.40398079,
    0.62031564,  -0.04972625, -0.58051979, 0.78587747,  -0.1139748,  -0.92311117, 0.85997613,
    -0.0739809,  -0.82336817, -0.83968132, -0.3051433,  -0.58523676, -1.08933726, -0.2612735,
    -0.26377806, -0.64656838, -0.27966684, -0.61519337, -0.45611229, -0.23014427, -0.55493106,
    -0.11446269, -0.18944839, -0.62403909, 0.45374863,  -0.2157663,  -1.0850038,  0.5435442,
    -0.27293322, -1.35297914, 0.51348898,  -0.19927139, -1.06256314, -0.94216533, -0.49330056,
    -1.21219849, -1.10857352, -0.41900866, -0.83209402, -1.13457746, -0.19642304, 0.0039321,
    -0.63434751, -0.1426404,  -0.11861352, 0.19098776,  -0.22969685, -0.96791793, 0.54908762,
    -0.17961939, -1.01303192, 0.34355934,  -0.13951417, -0.73370616, 0.40297637,  -0.21490147,
    -1.04928758, 0.59037956,  -0.19822379, -1.10797499, -0.66777329, -0.38678187, -0.99590924,
    -1.03109797, -0.36771059, -0.69290276, -0.95939632, -0.36583382, -0.73193751, -0.55844304,
    -0.31462113, -0.80033118, -0.0649215,  -0.25014555, -0.87921635, 0.50319565,  -0.20051826,
    -1.0605575,  0.52906666,  -0.26174396, -1.30251358, 0.40212034,  -0.22901988, -1.10071062,
    -0.52232329, -0.2629073,  -0.63311047, -0.65069093, -0.27541995, -0.59691854, -0.41994676,
    -0.2297763,  -0.57674981, -0.33876313, -0.19033597, -0.48358428, -0.17896712, -0.29743412,
    -0.98021561, 0.52383746,  -0.24905072, -1.25243761, 0.68269345,  -0.12383058, -0.89327557,
    0.61481454,  -0.14729485, -0.93615644, -0.85030941, -0.29723623, -0.5493199,  -0.7778087,
    -0.30926918, -0.64006992, -0.55157684, -0.31910865, -0.82124982, -0.59561683, -0.31916588,
    -0.79324159, -0.09168517, -0.18178513, -0.61042457, 0.37459956,  -0.0910014,  -0.57501417,
    0.37451994,  -0.14870511, -0.78737731, 0.36086716,  -0.11231039, -0.64465597, -0.63993921,
    -0.43585356, -1.19438279, -0.98786935, -0.39757419, -0.83053324, -0.87803347, -0.29701598,
    -0.53074473, -0.35157013, -0.14772893, -0.31853658, -0.08542878, -0.07267196, -0.21277498,
    0.05940196,  -0.1685065,  -0.65835453, 0.43258434,  -0.22802511, -1.11656876, 0.46119617,
    -0.19401331, -1.00970054, -0.685998,   -0.38416101, -0.97458392, -0.98620952, -0.32103516,
    -0.5498476,  -0.71851926, -0.40125735, -1.01667937, -0.63667588, -0.37628773, -0.97720497,
    -0.34150654, -0.23283315, -0.63826355, 0.41966502,  -0.13240373, -0.75629704, 0.48685384,
    -0.16799269, -0.93035592, 0.34018216,  -0.19140835, -0.92257081, -0.56286172, -0.21464273,
    -0.42946761, -1.06395512, -0.29064793, -0.38817259, -0.78311814, -0.27412652, -0.50730359,
    -0.92273453, -0.34833735, -0.69102222, -0.7303697,  -0.31601898, -0.69531362, 0.23443788,
    -0.21449511, -0.93979951, 0.35422505,  -0.24552722, -1.13078682, 0.34463082,  -0.15800211,
    -0.80244902, -0.8385257,  -0.34044994, -0.71594513, -0.95553531, -0.34480588, -0.65700515,
    -0.97502373, -0.31197721, -0.5236716,  -0.45077531, -0.24183009, -0.60136762, -0.03838397,
    -0.23733722, -0.84907149, -0.26293151, -0.3288665,  -1.04212117, 0.20744616,  -0.15118829,
    -0.68946449, 0.51460628,  -0.05885128, -0.54637607, 0.48948802,  -0.15382833, -0.87990311,
    0.62207217,  -0.15468564, -0.96801318, -1.17645898, -0.37772256, -0.63661693, -1.37271881,
    -0.42515765, -0.68547623, -1.20866043, -0.31485181, -0.38454898, -0.80315985, -0.28770475,
    -0.54444479, 0.0211727,   -0.32754229, -1.21928836, 0.39808612,  -0.21217812, -1.03612914,
    0.64282618,  -0.15003766, -0.96420169, 0.72864066,  -0.18696433, -1.15511936, -0.70142569,
    -0.31901534, -0.72488964, -1.16765876, -0.35161447, -0.54614876, -1.27316771, -0.39749009,
    -0.64741673, -0.53012519, -0.18237746, -0.3316715,  -0.47944949, -0.24412106, -0.59142777,
    -0.12583944, -0.26772123, -0.90488092, 0.60041291,  -0.09079496, -0.71894574, 0.63631314,
    -0.0583745,  -0.62260551, -0.45467017, -0.2841686,  -0.75472503, -0.66999498, -0.31064816,
    -0.71422853, -1.03665144, -0.47354562, -1.07893558, -0.93846456, -0.40745539, -0.89856357,
    -0.58656089, -0.17605595, -0.27223969, -0.30096477, -0.16417817, -0.41151391, 0.28224085,
    -0.22731645, -1.01762647, 0.40965083,  -0.26219121, -1.2276434,  -1.10559935, -0.56516142,
    -1.37200529, -1.54933105, -0.53327657, -0.97030919, -1.13293535, -0.30888426, -0.41110311,
    -0.35164,    -0.20239032, -0.51970684, 0.1486379,   -0.21915017, -0.9019584,  0.44066323,
    -0.19975965, -1.01769687, 0.38043432,  -0.27547838, -1.25783436, 0.42121411,  -0.2710847,
    -1.26779065};

float inputY[] = {
    6.72627056e-01,  3.69826234e-02,  -9.02181788e-17, 1.15215940e+00,  1.19119133e-01,
    -1.56858856e-16, 1.26133188e+00,  1.41772359e-01,  -1.72195174e-16, 1.03926143e+00,
    -1.38062725e-01, -1.31267156e-16, 6.69471981e-01,  -3.40943350e-01, -7.40678875e-17,
    2.86739218e-01,  -2.04505737e-01, -2.92891456e-17, 7.97118828e-02,  -1.08727196e-01,
    -5.98245477e-18, 6.25216534e-02,  -1.96070999e-02, -7.42648175e-18, 7.27962929e-02,
    1.08099000e-01,  -1.40979143e-17, 6.06863800e-01,  9.24637637e-02,  -8.38578843e-17,
    9.07325017e-01,  1.44802465e-01,  -1.25649422e-16, 8.17160253e-01,  1.56276642e-01,
    -1.14239900e-16, 6.42560918e-01,  7.22695627e-03,  -8.50154612e-17, 6.13720682e-01,
    -1.85895311e-02, -8.01383666e-17, 5.55894742e-01,  -6.87998425e-03, -7.30021612e-17,
    3.06250586e-01,  -1.76437465e-01, -3.30300810e-17, 9.42240879e-02,  -2.02196315e-01,
    -4.00429538e-18, 1.14860784e-01,  -4.03664102e-02, -1.34625397e-17, 1.46597026e-01,
    7.17329777e-02,  -2.23136881e-17, 5.37887505e-02,  8.51139218e-02,  -1.06350331e-17,
    7.95424459e-01,  -5.00716464e-02, -1.02783302e-16, 1.09991851e+00,  1.91582157e-02,
    -1.45809760e-16, 1.05607977e+00,  1.57635014e-01,  -1.45795360e-16, 7.34281050e-01,
    -7.06803390e-02, -9.38642075e-17, 5.51383869e-01,  -1.76039969e-01, -6.53647532e-17,
    4.93172896e-01,  2.97078018e-02,  -6.62562494e-17, 2.16817664e-01,  8.92895660e-02,
    -3.23024455e-17, -3.44049948e-02, 1.75576126e-02,  3.80493710e-18,  -1.12275706e-01,
    4.46752427e-03,  1.46163170e-17,  1.06670605e+00,  4.05612685e-02,  -1.42322144e-16,
    1.61324796e+00,  1.60113585e-01,  -2.19355040e-16, 1.39111547e+00,  1.43755896e-01,
    -1.89388292e-16, 6.28650320e-01,  -3.50178553e-01, -6.83015086e-17, 1.28246316e-01,
    -5.45858366e-01, 5.81805570e-18,  4.77769435e-02,  -1.72507255e-01, 8.83193541e-19,
    2.93244898e-02,  9.84828117e-02,  -7.96628164e-18, -1.95407143e-02, 8.83423794e-02,
    -1.10176246e-18, -2.84829409e-02, 7.06939587e-02,  8.11934726e-19,  5.80357284e-01,
    -4.62535874e-01, -5.72567906e-17, 1.13767723e+00,  -3.55028644e-01, -1.35209174e-16,
    1.29169850e+00,  -2.24629412e-02, -1.69361011e-16, 8.21477202e-01,  -1.87262404e-01,
    -1.00506356e-16, 4.12533011e-01,  -2.19745085e-01, -4.52392061e-17, 3.96345158e-01,
    1.09935539e-01,  -5.68307292e-17, 4.16216096e-01,  3.39313358e-01,  -6.90002636e-17,
    2.20823661e-01,  2.70167726e-01,  -4.03611583e-17, 1.80966938e-03,  1.69525018e-01,
    -7.29648164e-18, 1.46324725e-01,  -3.64138454e-01, -4.13099457e-18, 5.77625692e-01,
    -3.95789820e-01, -5.96755231e-17, 1.27408737e+00,  4.42597532e-02,  -1.69817067e-16,
    1.60142562e+00,  3.52360115e-01,  -2.25800263e-16, 1.27556382e+00,  1.51691007e-01,
    -1.74484445e-16, 6.40444807e-01,  -5.37033181e-02, -8.21997430e-17, 5.06182121e-02,
    -7.50426111e-02, -3.54917737e-18, -1.51261027e-01, 6.33963934e-02,  1.73026950e-17,
    -8.59675180e-02, 1.18603696e-01,  6.39599948e-18,  1.00560926e+00,  1.83002834e-01,
    -1.40197527e-16, 1.45819710e+00,  4.52440515e-02,  -1.94130887e-16, 1.34885700e+00,
    -7.66694087e-02, -1.74639931e-16, 8.59588359e-01,  -2.71236768e-01, -1.02034757e-16,
    2.96212323e-01,  -3.23490988e-01, -2.55843139e-17, -8.88367997e-02, -2.36814414e-01,
    2.15715340e-17,  -1.85995627e-01, -1.04198585e-01, 2.88596120e-17,  -6.75487786e-02,
    4.42778897e-02,  7.06212489e-18,  1.16589455e+00,  3.46311834e-01,  -1.68128480e-16,
    1.56360293e+00,  6.13127395e-02,  -2.08696477e-16, 1.36520189e+00,  -1.61421143e-01,
    -1.73266325e-16, 7.95497934e-01,  -2.51493707e-01, -9.44071106e-17, 1.04566338e-01,
    -3.11870789e-01, -8.01689846e-19, -1.93481979e-01, -1.41476337e-01, 3.13986033e-17,
    -1.09909015e-01, 7.77108219e-02,  1.12549300e-17,  -2.10671441e-03, 9.45970584e-02,
    -3.66064662e-18, 1.27338566e+00,  1.46627910e-02,  -1.68492334e-16, 1.74028739e+00,
    3.18033866e-02,  -2.30761803e-16, 1.23715331e+00,  -2.90909806e-01, -1.50993482e-16,
    6.25294537e-01,  -5.22313572e-01, -6.06925258e-17, 3.39160685e-01,  -1.63375447e-01,
    -3.79127308e-17, 1.87771210e-01,  2.95181046e-01,  -3.70449453e-17, 6.46105379e-02,
    2.93939388e-01,  -2.07558731e-17, 8.61382871e-01,  -1.32420884e-01, -1.08050716e-16,
    1.15895000e+00,  -3.16485442e-01, -1.39618437e-16, 1.20580628e+00,  -1.94282835e-01,
    -1.50883627e-16, 1.04195045e+00,  9.99080854e-02,  -1.41529199e-16, 5.36684279e-01,
    1.86332787e-01,  -7.85135708e-17, 7.27011981e-02,  6.30315868e-02,  -1.22090691e-17,
    -2.55125254e-02, 7.33264056e-02,  3.10720543e-19,  2.01436646e-02,  9.50710745e-02,
    -6.61384887e-18, 7.80878674e-01,  -1.41926397e-01, -9.70413778e-17, 1.06780232e+00,
    -2.49872816e-01, -1.30374927e-16, 1.04868227e+00,  -2.14032294e-01, -1.29346320e-16,
    8.35316907e-01,  -1.75150351e-01, -1.02835234e-16, 6.39974154e-01,  5.81841766e-02,
    -8.67959453e-17, 4.64459638e-01,  2.97691394e-01,  -7.36277809e-17, 1.65867218e-01,
    1.94150002e-01,  -2.99508831e-17, 3.40941159e-02,  9.62950893e-02,  -8.50402205e-18,
    7.04609207e-01,  -1.40590121e-01, -8.70417224e-17, 1.06865641e+00,  -9.25513357e-02,
    -1.37037353e-16, 1.05507003e+00,  -6.89795939e-02, -1.36227509e-16, 9.28564181e-01,
    -2.16795100e-01, -1.13395049e-16, 8.05337648e-01,  -9.29064623e-02, -1.02306889e-16,
    5.03009198e-01,  1.22844210e-01,  -7.14306393e-17, 1.75321450e-01,  1.69975948e-01,
    -3.01908723e-17, 5.67295094e-03,  1.07148867e-01,  -5.20888382e-18, 1.49232863e-02,
    9.48304027e-02,  -5.91557953e-18, 5.43436937e-01,  -7.74917305e-01, -3.93837600e-17,
    1.15235436e+00,  -7.50890482e-01, -1.20663132e-16, 1.67352530e+00,  -1.08152516e-03,
    -2.20590848e-16, 1.52125445e+00,  3.84514866e-01,  -2.16569288e-16, 8.81241613e-01,
    4.90289337e-01,  -1.36594393e-16, 1.50786852e-01,  3.11460219e-01,  -3.28467218e-17,
    -5.03902739e-02, 1.51941642e-01,  3.17560937e-19,  1.64457512e-01,  3.77258994e-01,
    -3.73884722e-17, 9.72215636e-01,  -1.12334433e-01, -1.23499062e-16, 1.28083555e+00,
    -3.29402626e-01, -1.55149919e-16, 1.03216298e+00,  -3.22065484e-01, -1.22670650e-16,
    6.03713399e-01,  -3.09468587e-01, -6.67087431e-17, 4.79213588e-01,  -1.18515053e-01,
    -5.82448523e-17, 3.51295656e-01,  1.20509801e-01,  -5.13316908e-17, 1.07750479e-01,
    9.51154181e-02,  -1.81656885e-17, 3.85669759e-02,  6.68281091e-02,  -7.86690988e-18,
    5.25723185e-01,  -7.29188671e-01, -3.89522337e-17, 1.01161423e+00,  -4.20698102e-01,
    -1.15855115e-16, 1.30942611e+00,  5.57457503e-02,  -1.74954292e-16, 9.99592908e-01,
    3.69927823e-02,  -1.33325448e-16, 6.13815801e-01,  2.61042412e-03,  -8.10335324e-17,
    2.52563454e-01,  1.24423731e-01,  -3.84778877e-17, 4.29922074e-02,  1.55613237e-01,
    -1.21467514e-17, 4.39238415e-02,  7.75618996e-02,  -9.02003740e-18, 6.74477255e-01,
    -8.71903455e-02, -8.52923688e-17, 1.06886010e+00,  -1.32566013e-01, -1.35398261e-16,
    1.12801321e+00,  -1.25094390e-01, -1.43508015e-16, 8.63440139e-01,  -2.79594396e-01,
    -1.02194616e-16, 5.52385103e-01,  -2.71398995e-01, -6.15266379e-17, 2.99290500e-01,
    -6.32828080e-02, -3.68234794e-17, 3.88777957e-02,  2.80746528e-02,  -6.29445132e-18,
    9.51386231e-02,  1.66658971e-01,  -1.94815529e-17, 2.00186927e-01,  3.17497470e-01,
    -3.96109287e-17, 6.72780758e-01,  -3.30384836e-01, -7.49436998e-17, 1.26631429e+00,
    -1.46317528e-01, -1.60857899e-16, 1.51707019e+00,  2.50897986e-01,  -2.10454719e-16,
    1.06149439e+00,  1.29118198e-01,  -1.45321968e-16, 4.41678872e-01,  -1.80109579e-01,
    -5.07319263e-17, 9.69401895e-02,  -1.75369396e-01, -5.47927830e-18, -1.12514563e-01,
    -3.65201242e-03, 1.49858513e-17,  -1.36401109e-01, 5.60400564e-02,  1.56498476e-17,
    -2.36885591e-02, 9.31352935e-02,  -7.54459958e-19, 8.60562569e-01,  -4.96950146e-01,
    -9.27659888e-17, 1.15911633e+00,  -7.03482276e-01, -1.23528386e-16, 1.16576111e+00,
    -3.64952676e-01, -1.38498554e-16, 7.62400183e-01,  -1.55661264e-01, -9.40333637e-17,
    2.76009568e-01,  5.83734613e-02,  -3.88191046e-17, 2.81602963e-01,  5.74772645e-01,
    -6.10559670e-17, 1.45604564e-01,  5.33480925e-01,  -4.14069630e-17, -6.58073308e-02,
    2.18900211e-01,  -4.37580381e-19, 7.97702828e-01,  5.01155500e-02,  -1.07254809e-16,
    1.08526832e+00,  -3.22798807e-01, -1.29641473e-16, 1.10315886e+00,  -4.12369566e-01,
    -1.28271011e-16, 9.90231311e-01,  -1.76224875e-01, -1.23214256e-16, 7.55266483e-01,
    2.65592962e-02,  -1.00679319e-16, 3.09795369e-01,  -8.30913302e-02, -3.73837353e-17,
    9.18225171e-02,  -3.13541397e-02, -1.08004091e-17, 1.47032928e-01,  1.93290913e-01,
    -2.74320227e-17, 1.02967528e+00,  -5.18922713e-01, -1.14146845e-16, 1.47570579e+00,
    -5.15449056e-01, -1.73095674e-16, 1.24550916e+00,  -4.05494447e-02, -1.62518453e-16,
    5.90302383e-01,  1.57311902e-01,  -8.43742901e-17, 4.77998886e-02,  6.63999906e-02,
    -9.06634370e-18, 3.23521397e-02,  8.34236682e-02,  -7.73848113e-18, 9.88247362e-02,
    1.73567769e-01,  -2.02551628e-17, 6.51701265e-01,  -3.66158984e-01, -7.06752027e-17,
    1.03579992e+00,  -4.22703631e-01, -1.18960235e-16, 1.25333100e+00,  -1.90572209e-01,
    -1.57303724e-16, 1.12108717e+00,  2.36499076e-02,  -1.48787619e-16, 6.97954362e-01,
    2.23505587e-01,  -1.01322883e-16, 3.79107058e-01,  3.04306594e-01,  -6.26503964e-17,
    1.76541250e-01,  1.99562284e-01,  -3.15834682e-17, -2.19222391e-02, 4.89746500e-02,
    8.51226330e-19};

float inputT[] = {0.00166667, 0, 0, 0, 0.00166667, 0, 0, 0, 0.00166667};

float inputP = 0.0625;

float gold_out[] = {-0.34156891, -0.23694405, -0.90949887, -0.23694405, 0.95815162,
                    -0.16063308, -0.90949887, -0.16063308, 0.38341729};

float gold_out_sink[M][P];