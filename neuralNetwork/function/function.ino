//this can train and run a custom neural network having an architecture consists of one hidden layer only along with input and output layers on a arduino 
//activations in order; sigmoid, linear
#include <math.h>

//#define DEBUG/

const int numberOfDataSet = 2; 
const int inputNodes = 9;
const int hiddenNodes = inputNodes+10;
const int outputNodes = 9;

//the training data because I can't be bothered to write a file handling system for this
//0 is min value of the sensor and 1 is the max value of the sensor
double Input[numberOfDataSet][inputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

const double Target[numberOfDataSet][outputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

int i, j, p;
double Accum;

double Hidden[hiddenNodes];
double Output[outputNodes];

double HiddenWeights[inputNodes + 1][hiddenNodes] = //{{2.0090010166168213, 0.9164603352546692, -0.39176902174949646, -2.0763261318206787, -6.089452743530273, 0.5568827390670776, -2.685368776321411, 1.698843002319336, -0.3029080331325531, -1.3712427616119385, -0.8833332657814026, 3.916396141052246, -2.7014622688293457, 1.4329383373260498, -0.9265817999839783, 2.373422384262085, -2.847163200378418, -9.086898803710938, -1.386370062828064}, {0.4953332841396332, -0.9608941078186035, -0.5340107679367065, 0.310898095369339, 1.3500486612319946, -8.589598655700684, -1.1691923141479492, -1.0093225240707397, 0.7737630009651184, 0.9383821487426758, -0.10471032559871674, 1.7654484510421753, 0.9555957913398743, 2.008375406265259, 0.6281931400299072, 0.660741925239563, -1.2063407897949219, 4.230349540710449, -1.764047622680664}, {5.429308891296387, -2.988046407699585, 8.209311485290527, 0.35200634598731995, -3.3422937393188477, 0.177134707570076, 0.2387770563364029, -1.0656768083572388, 1.8831266164779663, 1.017304539680481, -1.558264970779419, 6.915225505828857, 0.6476768851280212, 0.0020668460056185722, -2.497277021408081, 1.815761923789978, -0.3304728865623474, -0.7459255456924438, -0.39775681495666504}, {0.39732593297958374, -0.17960011959075928, -3.000454902648926, -2.0826144218444824, 0.06144428998231888, -2.2790908813476562, 0.9520445466041565, -1.1093873977661133, 2.6819052696228027, -0.13331060111522675, -0.9682339429855347, -0.5289739966392517, -0.781704306602478, -0.6360613703727722, -0.8630073070526123, -1.1665278673171997, 0.7061499357223511, -1.553481936454773, -0.44279828667640686}, {-8.61706256866455, -0.06795011460781097, -6.239126205444336, -2.215949058532715, 2.116220474243164, 1.268202543258667, -0.4362614154815674, -3.365438938140869, -0.32875141501426697, 5.0761542320251465, -1.3767017126083374, -1.1911348104476929, -2.4203414916992188, -0.26274576783180237, -0.582804799079895, -0.5734472274780273, -0.3099827170372009, 3.2326619625091553, -1.941918134689331}, {0.1513442099094391, -1.7635905742645264, -2.989370346069336, 1.2805249691009521, -0.607323944568634, 5.287376880645752, 0.5918378233909607, 3.1000545024871826, -1.3972339630126953, -2.626469612121582, -1.0928294658660889, -0.8159122467041016, -0.1752488613128662, -1.690161943435669, 2.254523754119873, -2.044950246810913, -0.6950883865356445, 0.9283224940299988, -0.8415866494178772}, {-1.9183173179626465, 1.1422663927078247, 0.2814454436302185, 1.8771101236343384, -0.5270687937736511, 3.314211130142212, 2.9658358097076416, 3.0495636463165283, -0.9927619099617004, -2.4697749614715576, 1.695864200592041, -1.8881574869155884, 0.4090156555175781, -0.5713328719139099, 3.0373342037200928, 0.25592559576034546, 1.6891748905181885, 0.6328374743461609, 0.6954604983329773}, {0.12588411569595337, 1.44865083694458, 2.2504994869232178, -1.8804569244384766, 0.5802839994430542, 0.9270026087760925, -1.7256635427474976, 2.3286657333374023, -1.0647975206375122, 0.8708268404006958, -1.3085353374481201, -0.5969376564025879, -0.20916806161403656, 0.038543783128261566, -3.2963063716888428, 2.4817914962768555, 0.847817599773407, 2.0159876346588135, 1.1827924251556396}, {1.6754847764968872, -1.3402462005615234, 2.835606813430786, 0.5729682445526123, -0.9644140601158142, -0.35159823298454285, 0.23683534562587738, 0.5110614895820618, 3.622615337371826, -3.0980241298675537, 1.0741862058639526, -2.0249998569488525, -1.8034756183624268, -3.0424787998199463, 1.444130301475525, 0.28939592838287354, -0.7714011073112488, 1.100798487663269, 1.2339500188827515}, {0.7828726172447205, -0.407754510641098, -0.4411654770374298, -0.17818091809749603, 0.9317843914031982, -1.385932445526123, -0.4227554202079773, -2.077815055847168, -2.526827096939087, -0.27255192399024963, -0.4498231112957001, -2.4062178134918213, 2.0947508811950684, 0.3301967978477478, -0.47391554713249207, -0.5139546990394592, -0.9728932976722717, -4.389845371246338, 0.5336097478866577}};
{{0.3569278419017792, 0.4111080765724182, -0.0049735670909285545, -0.8488397598266602, -0.41836032271385193, 0.26787447929382324, -0.546505868434906, 1.0109919309616089, 0.14223292469978333, -0.12681709229946136, -0.07890532910823822, 0.4951496720314026, -0.173179030418396, 0.10641241818666458, -0.3498278260231018, 0.6032876372337341, -0.3584327697753906, -0.8169934153556824, -0.20165616273880005}, {-0.7381668090820312, -0.16608396172523499, 0.0024467010516673326, -0.14678341150283813, -0.8553261160850525, -1.6036553382873535, -0.35727864503860474, 0.2822601795196533, 0.11051187664270401, 0.8239600658416748, 0.5997465252876282, 0.014689474366605282, 0.04340163245797157, -0.25482890009880066, 0.2076227068901062, 0.2976023256778717, -0.4718233346939087, 0.7521365284919739, -0.42964720726013184}, {0.23842398822307587, -0.308176726102829, 0.875759482383728, 0.43940773606300354, -1.064095139503479, 0.01651560515165329, -0.02304415963590145, -0.6065678596496582, 0.8076603412628174, -0.6569377183914185, 0.03904074430465698, 1.1989728212356567, 0.3275788724422455, 0.05284455046057701, -0.9214649796485901, 0.3366106152534485, -0.5357016921043396, -0.9728436470031738, 0.2947288751602173}, {0.2363746464252472, 0.4849419593811035, -0.36111459136009216, -0.4237004220485687, -0.05957760289311409, -0.5570628046989441, 0.20212812721729279, 0.350588321685791, 1.456301212310791, 0.07805357873439789, -0.24829307198524475, -1.1202195882797241, -0.3982192873954773, -1.2005962133407593, 0.08714661747217178, -0.3506779968738556, -0.317238450050354, 0.1826644241809845, -0.8043909668922424}, {-0.7918895483016968, -0.38346314430236816, -0.3901078701019287, 0.046368762850761414, -0.16612549126148224, -0.6565540432929993, -0.24213401973247528, 0.21017156541347504, -0.19684560596942902, 0.9644094109535217, -0.2766595482826233, -0.051300860941410065, -0.6281813979148865, 0.12208807468414307, -0.32665133476257324, 0.32926005125045776, 0.13199198246002197, 0.35239511728286743, -0.4485349953174591}, {0.6392689347267151, -0.3270547688007355, 0.2497664988040924, -0.18960580229759216, -0.1490742266178131, 0.1138807013630867, 0.571025013923645, -0.2605089545249939, -0.6915791034698486, -0.8679197430610657, -0.7118017077445984, -0.08229729533195496, -0.26401224732398987, 0.33563077449798584, 0.39133551716804504, 0.22625818848609924, 0.5779123902320862, 0.18530672788619995, -0.3204455077648163}, {0.24502968788146973, 0.16983185708522797, -0.5536826848983765, 0.11553884297609329, 0.09389497339725494, 0.4248955249786377, -0.04724910855293274, 0.7091777324676514, -0.04122794419527054, -0.4770837128162384, 0.12286565452814102, -0.10578683018684387, 0.41233599185943604, -0.5669387578964233, 0.20590445399284363, -0.6038642525672913, -0.27463632822036743, -0.38393622636795044, 0.2902030646800995}, {-0.2262842357158661, -0.24127505719661713, 0.2518789768218994, -0.7467154860496521, 0.24521711468696594, 1.0085139274597168, 0.04690830782055855, 0.014065570197999477, -0.7619898319244385, -0.10989490151405334, -0.2592472732067108, 0.04040851071476936, -0.5013391971588135, -0.2977566421031952, 0.0058630057610571384, 0.49352994561195374, 0.5646018981933594, 0.016173236072063446, 0.5212085843086243}, {0.37584197521209717, 0.5298487544059753, 0.029990270733833313, 0.36450809240341187, -0.2099698781967163, -0.3569125235080719, 0.368763267993927, -0.26239097118377686, 0.696457028388977, -0.7423338890075684, 0.48590975999832153, -0.4578595459461212, -0.08937324583530426, -0.19260098040103912, 0.2670210301876068, 0.2502375543117523, 0.03298167139291763, 0.17619362473487854, 0.34039777517318726}, {-0.6048238277435303, -0.10073967278003693, -0.15524081885814667, -0.11458954960107803, 0.06380163133144379, -0.04202279448509216, -0.06737372279167175, -0.454315721988678, -0.1308087259531021, -0.2472197562456131, 0.022040851414203644, 0.24464228749275208, -0.06870073825120926, 0.28088125586509705, -0.053192079067230225, 0.11798659712076187, -0.08334149420261383, 0.03664485737681389, 0.06256876140832901}};

double OutputWeights[hiddenNodes + 1][outputNodes] = //{{-4.814497515326366e-05, -4.198963870294392e-05, -2.0170009520370513e-05, 9.161160414805636e-05, -6.549164390889928e-05, 1.4767723083496094, -0.17904455959796906, 0.1323123723268509, 0.3085419833660126}, {0.0002306718088220805, 0.00018176813318859786, 0.00010452545393491164, -0.00036102556623518467, 0.00040888809598982334, -1.5727566480636597, 0.2868851125240326, 1.5543153285980225, -0.35929954051971436}, {-7.861662743380293e-05, -6.341216794680804e-05, -3.5025870602112263e-05, 0.00012890504149254411, -0.00013218022650107741, 2.4079689979553223, 0.33807119727134705, 0.07030351459980011, -0.2732928693294525}, {0.000434454414062202, 0.0003363058785907924, 0.000199280766537413, -0.0006556370062753558, 0.0007996102212928236, -1.7908908128738403, -0.8064109086990356, -0.7001121044158936, -0.6158998012542725}, {3.188733899150975e-05, 2.7352407414582558e-05, 1.3558607861341443e-05, -5.886712096980773e-05, 4.5649863750441e-05, 0.04094704985618591, -1.965779423713684, -1.8072339296340942, -0.5661985278129578}, {-0.00013146104174666107, -0.00010531296720728278, -5.885507562197745e-05, 0.00021263329836074263, -0.00022455181169789284, 1.3721387386322021, -1.2943110466003418, -0.45569363236427307, 0.37303426861763}, {0.00015966041246429086, 0.0001302734308410436, 7.0534399128519e-05, -0.0002677950542420149, 0.00026114724460057914, 1.0835336446762085, -0.5626476407051086, 0.7747160792350769, 0.0459759496152401}, {0.00013227084127720445, 0.00010454701987328008, 5.979200068395585e-05, -0.00020826247055083513, 0.00023286625219043344, -1.2780532836914062, 0.3266037702560425, -0.07540303468704224, -1.1865049600601196}, {-4.0477556467521936e-05, -3.544625360518694e-05, -1.6885944205569103e-05, 7.757449930068105e-05, -5.432195030152798e-05, -0.7410312294960022, -0.0430673211812973, 1.3372637033462524, -1.0121793746948242}, {0.0001467679685447365, 0.00011410594743210822, 6.710781599394977e-05, -0.00022344816534314305, 0.00026767130475491285, 0.7589191198348999, 1.3479821681976318, -0.21967165172100067, -1.778529405593872}, {-0.0019615760538727045, -0.0015395120717585087, -0.0008912053890526295, 0.0030448378529399633, -0.0035070155281573534, -1.0469545125961304, 1.426551342010498, 0.11587999016046524, 1.0225826501846313}, {5.003419209970161e-05, 4.269933560863137e-05, 2.133858833985869e-05, -9.143358329311013e-05, 7.264789019245654e-05, -2.011577606201172, -0.871758759021759, -1.4867933988571167, -0.39546728134155273}, {0.0001578928786329925, 0.00013598402438219637, 6.684571417281404e-05, -0.00029352999990805984, 0.00022321258438751101, -2.104071617126465, -1.181106448173523, -0.42705076932907104, 0.21912430226802826}, {-0.00025522595387883484, -0.00020995487284380943, -0.00011202124733245, 0.00043484766501933336, -0.0004090112051926553, 1.2381184101104736, 0.4920274019241333, 1.0256117582321167, 1.7790125608444214}, {-6.593630678253248e-05, -5.737452374887653e-05, -2.767721343843732e-05, 0.00012494016846176237, -9.03374093468301e-05, 0.8333255052566528, 0.3623029887676239, -0.8944454789161682, -0.17623794078826904}, {-0.00025482941418886185, -0.000203374118427746, -0.00011439745139796287, 0.000409086438594386, -0.0004390432150103152, 1.3604650497436523, -0.5407445430755615, -1.2066388130187988, 0.9340253472328186}, {-0.0008859836379997432, -0.0007386388606391847, -0.0003850174543913454, 0.0015491617377847433, -0.0013721009017899632, 1.213974952697754, 1.3904508352279663, 0.22060295939445496, 0.8706419467926025}, {-0.0001745242770994082, -0.00013995343761052936, -7.808620284777135e-05, 0.0002828802680596709, -0.00029743017512373626, -0.2560468912124634, 1.246046781539917, 1.057081699371338, 2.319755792617798}, {0.0007918431074358523, 0.0006273192120715976, 0.0003573775466065854, -0.0012526160571724176, 0.0013870194088667631, -0.8750327229499817, 0.240896537899971, -0.04583832249045372, 2.1072230339050293}, {0.00016427173977717757, 0.00013755627151113003, 7.111677405191585e-05, -0.0002896074438467622, 0.00025138427736237645, -0.17553949356079102, 0.9542275071144104, 1.112914800643921, 0.8840166926383972}};
{{-0.12527534365653992, 0.07475017011165619, -0.013676805421710014, -0.08092579245567322, -0.3947520852088928, 0.979529082775116, -0.6380037069320679, 0.5972855687141418, -0.695248544216156}, {-0.2674000561237335, 0.09960779547691345, 0.40398213267326355, -0.07284115999937057, 0.2772495746612549, 0.0618208572268486, -0.25581005215644836, 0.6272822022438049, -0.4313372075557709}, {0.03971487283706665, 0.279960036277771, -0.14479276537895203, 0.20517723262310028, -0.14087985455989838, 0.33655720949172974, -0.4875926673412323, -0.7422268986701965, -0.10517358779907227}, {-0.23338791728019714, -0.3006652295589447, 0.27215519547462463, 0.16149267554283142, 0.06900016963481903, -0.6763208508491516, -0.5530382990837097, -0.07008112967014313, 0.5109456181526184}, {-0.34462693333625793, -0.26183515787124634, -0.2687077522277832, 0.38987088203430176, -0.30507928133010864, 0.3022163212299347, -0.6685993671417236, 0.058157891035079956, 0.9531700015068054}, {0.19568492472171783, -0.07646976411342621, 0.03639793395996094, -0.3409585654735565, -0.021182656288146973, 1.194447636604309, -0.565788984298706, -1.0184614658355713, 0.5665853023529053}, {-0.1461782157421112, 0.1785563975572586, 0.21024449169635773, -0.2008543461561203, -0.44804811477661133, 0.41825178265571594, -0.239919051527977, 0.4041219651699066, 0.4945337176322937}, {0.07180728018283844, 0.1508767008781433, -0.22612157464027405, 0.16212506592273712, -0.17033906280994415, -0.30580639839172363, 0.9066464304924011, 0.6133001446723938, -1.1737031936645508}, {0.4562338888645172, 0.047469187527894974, -0.21422111988067627, -0.18146571516990662, 0.28166648745536804, -0.06217662990093231, -0.4689127802848816, 0.790641725063324, -1.537990927696228}, {0.0664065033197403, 0.19311045110225677, 0.21777649223804474, -0.2446144074201584, -0.2945227026939392, -0.6158235669136047, 1.6651701927185059, -0.056425634771585464, -0.06254328042268753}, {-0.2428378462791443, -0.3280857801437378, 0.011219964362680912, 0.5015391707420349, -0.36142945289611816, -0.8462530970573425, -0.09721960127353668, -0.11444760859012604, 0.17182789742946625}, {-0.37948161363601685, -0.257948100566864, 0.3281254470348358, -0.09082470089197159, 0.1960752010345459, -0.13824714720249176, -0.20612888038158417, -1.7063311338424683, 0.407747358083725}, {0.1898241639137268, 0.2924923002719879, -0.13264599442481995, -0.16329044103622437, 0.2615583837032318, -0.35985660552978516, -0.7614031434059143, -0.21172824501991272, -0.017389217391610146}, {0.41068923473358154, 0.09621012210845947, -0.17535896599292755, -0.06850939244031906, 0.12651467323303223, -0.2502007782459259, -0.13285218179225922, -0.7117354869842529, 1.216811180114746}, {0.19704796373844147, -0.21059513092041016, 0.12301681935787201, -0.18685029447078705, 0.1698504090309143, 0.35421985387802124, 0.5111345052719116, 0.7873338460922241, 0.442974716424942}, {-0.1346028447151184, -0.3934791088104248, 0.03606453910470009, -0.009897958487272263, -0.11397114396095276, 0.36998218297958374, 0.4354320764541626, -0.6042404770851135, 0.32129913568496704}, {-0.09515606611967087, 0.013449053280055523, 0.22663362324237823, 0.44161510467529297, 0.5550313591957092, 0.5544446110725403, 0.2780064046382904, 0.20383788645267487, 0.7978240847587585}, {-0.0111014973372221, 0.08037968724966049, -0.2556232810020447, -0.287820965051651, 0.016286011785268784, -0.6170938611030579, 0.6244246363639832, 0.733364462852478, 0.7539536356925964}, {0.4269564747810364, 0.23368462920188904, -0.2163955122232437, -0.2915840446949005, -0.006522768642753363, -0.10181216150522232, -0.4974723160266876, -0.5756582617759705, 0.7169337272644043}, {-0.033449601382017136, 0.08068053424358368, -0.09040544182062149, 0.19358612596988678, 0.099988654255867, 0.06006380543112755, 0.13653108477592468, -0.19894346594810486, 0.2627280056476593}};

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(A15));       //Collect a random ADC sample for Randomization.
}

void loop() 
{
  //trainNN();
  driveNN();

}


void driveNN()
{
  Serial.println("Running NN Drive Test");
  
  while (true) 
  {
    double TestInput[inputNodes];
    //raw inputs go here
    //take analog inputs from a0 to a8
  

    int tem;

    //mapping of each finger to [0, 1]

    //index finger
    //[0]
    tem = analogRead(A0);
    tem = map(tem, 460, 850, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[0] = double(tem)/100;
  //{1}
    tem = analogRead(A5);
  
    tem = map(tem, 650, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[5] = double(tem)/100;
  


    //middle finger
    //[0]
    tem = analogRead(A1); 
    tem = map(tem, 350, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[1] = double(tem)/100;
  
    //[1]
    tem = analogRead(A6);
    tem = map(tem, 650, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[6] = double(tem)/100;

    //ring finger
    //[0]
    tem = analogRead(A2);
    tem = map(tem, 530, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[2] = double(tem)/100;
    //[1]
    tem = analogRead(A7);
    tem = map(tem, 570, 870, 0, 100);
    tem =  constrain(tem, 0, 100);
    TestInput[7] = double(tem)/100;

    //pinky finger
    // [0]

    tem = analogRead(A3);
    tem = map(tem, 300, 770, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[3] = double(tem)/100;

    //[1]
    tem = analogRead(A8);
    tem = map(tem, 570, 870, 0, 100);
    tem =  constrain(tem, 0, 100);
    TestInput[8] = double(tem)/100;

    //thumb
    // [0]
    tem = analogRead(A4);
    tem = map(tem, 350, 420, 100, 0);
    tem = constrain(tem, 0, 100);
    TestInput[4] = double(tem)/100;


  #ifdef DEBUG
      for(i = 0; i<numberOfDataSet; i++)
      {
        Serial.print(TestInput[9-i], 2);
        Serial.print("Input: ");
        Serial.print("\t");
      }
  #endif
  
    InputToOutput(TestInput); //INPUT to the function to obtain OUTPUT

    
    int bin = 0;
    int decValue = 0;
    int base = 256; 
    for(i = 0; i<outputNodes; i++)
    {
        bin = Output[i] > 0.5 ? 1: 0;
 
        decValue += bin * base;
        base = base/ 2;
//    Serial.print(Output[i] > 0.5 ? 1: 0);
//    Serial.print(":");
    }
    
    Serial.println(decValue);
    delay(200);
//  Serial.println();/
  }
}

//the function for which this whole circus is established
void InputToOutput(double TestInput[])
{

  //Compute hidden layer activations
  for ( i = 0 ; i < hiddenNodes ; i++ ) 
  {
    Accum = HiddenWeights[inputNodes][i];
    for ( j = 0 ; j < inputNodes ; j++ ) 
    {
      Accum += TestInput[j] * HiddenWeights[j][i];
    }
    Hidden[i] = 1.0 / (1.0 + exp(-Accum));
    //(Accum>0? Accum: 0);
    //1.0 / (1.0 + exp(-Accum));
  }

  //Compute output layer activations
  for ( i = 0 ; i < outputNodes ; i++ ) 
  {
    Accum = OutputWeights[hiddenNodes][i];
    for ( j = 0 ; j < hiddenNodes ; j++ ) 
    {
      Accum += Hidden[j] * OutputWeights[j][i];
    }
    
    Output[i] = Accum;
//  Accum;
//    (Accum>0? Accum: 0);
  }

  #ifdef DEBUG
    Serial.print ("  Output ");
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
      Serial.print (Output[i], 5);
      Serial.print (" ");
    }
    Serial.println();
  #endif
}



//For displaying information
void toTerminal()
{
  for ( p = 0; p < numberOfDataSet; p++ ) 
  {
    Serial.println();
    Serial.print("  Training Pattern: ");
    Serial.println(p);
    Serial.print ("  Input ");
    for ( i = 0 ; i < inputNodes ; i++ ) 
    {
      Serial.print(Input[p][i], DEC);
      Serial.print(" ");
    }
    Serial.print("  Target ");
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
      Serial.print(Target[p][i], DEC);
      Serial.print(" ");
    }

    //Compute hidden layer activations
    for ( i = 0 ; i < hiddenNodes ; i++ ) 
    {
      Accum = HiddenWeights[inputNodes][i] ;
      for ( j = 0 ; j < inputNodes ; j++ ) 
      {
        Accum += Input[p][j] * HiddenWeights[j][i] ;
      }
      //activation function
      Hidden[i] = 1.0 / (1.0 + exp(-Accum));
      //1.0 / (1.0 + exp(-Accum));
    }
    
    //Compute output layer activations and calculate errors
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
      Accum = OutputWeights[hiddenNodes][i] ;
      for ( j = 0 ; j < hiddenNodes ; j++ ) 
      {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }

      //linear activation
      Output[i] = Accum ;
    }

    Serial.print("  Output ");
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
     Serial.print(Output[i], 5);
     Serial.print(" ");
    }
  }
}
