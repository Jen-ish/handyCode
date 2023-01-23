//this can be used to test the weights after training
//basically it acts as a function; insert input and get output
//the architecture consists of one hidden layer only along with input and output layers
#include <iostream>
#include <math.h>
#include <random>

/******************************************************************
   Network Configuration - customized per network
*******************************************************************/

const int PatternCount = 2;
const int InputNodes = 9;
const int HiddenNodes = InputNodes + 10;
const int OutputNodes = 9;


//the training data because I can't be bothered to write a file handling system for this

//0 is min value of the sensor and 1 is the max value of the sensor
double Input[PatternCount][InputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

const double Target[PatternCount][OutputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

/******************************************************************
   End Network Configuration
*******************************************************************/

unsigned i, j; //arduino code relic; just ignore these two 
double Accum;

double Hidden[HiddenNodes];
double Output[OutputNodes];

double HiddenWeights[InputNodes + 1][HiddenNodes] = {{0.40788745880126953, -0.4361528158187866, -0.3479953408241272, -0.3236440420150757, 2.125603675842285, 0.30508682131767273, -0.43863165378570557, -1.9835561513900757, 0.26741790771484375, -1.1680402755737305, -0.3649548292160034, -0.22882433235645294, 0.06189138814806938, 0.05408106744289398, -0.20584221184253693, 0.42881396412849426, -0.6486574411392212, 0.25485068559646606, 1.362567663192749}, {0.3247884511947632, -0.08729679137468338, -0.3908969759941101, -0.32884007692337036, 0.8438133597373962, 0.29473909735679626, -0.06706410646438599, -1.3936179876327515, -0.37878671288490295, -0.6101034283638, -0.36609360575675964, -0.11825667321681976, 0.2052663415670395, 0.27089089155197144, -0.4710594713687897, -0.7385116219520569, -1.068861722946167, 0.07564198970794678, 0.1554359346628189}, {0.31980472803115845, 0.010021121241152287, -0.18437203764915466, -0.386432021856308, 1.9449881315231323, 0.1246812641620636, 0.36428117752075195, 1.990365982055664, -0.4748932421207428, -0.24427156150341034, -0.24672454595565796, 0.36001238226890564, -0.37480777502059937, -0.5606327652931213, 0.24112021923065186, -0.8544895052909851, -0.17252054810523987, 0.05596578121185303, -1.5149484872817993}, {-0.22201351821422577, 0.27805620431900024, 0.01893553137779236, -0.06971466541290283, -0.979842483997345, -0.16433921456336975, -0.45366278290748596, 1.3425416946411133, 0.14429719746112823, 0.3991241753101349, 0.04026937484741211, -0.029295269399881363, 0.17571794986724854, 1.5624545812606812, 0.16674116253852844, -1.0375803709030151, 0.06589832156896591, -0.2266821265220642, 0.3891756236553192}, {-0.3843138813972473, -0.13446837663650513, 0.16734082996845245, -0.4129665195941925, 0.08948110044002533, 0.2699219286441803, -0.32249224185943604, -0.15135538578033447, 0.11070646345615387, 1.1356220245361328, -0.29275351762771606, 0.2382863163948059, -0.2934669852256775, -2.0915045738220215, -0.34841832518577576, -0.5085658431053162, 0.3997286856174469, -0.2827707529067993, -0.5135178565979004}, {-0.31865692138671875, -0.3205561339855194, -0.33792179822921753, -0.11176601052284241, -0.4950488805770874, -0.4702107012271881, -0.1149592399597168, 1.4680266380310059, -0.24831759929656982, 0.4582294821739197, -0.42668628692626953, -0.46075406670570374, 0.4018532335758209, 1.14976966381073, -0.3663412034511566, 0.36354926228523254, 0.42889824509620667, 0.2915871739387512, 2.680306911468506}, {-0.29586857557296753, 0.29189714789390564, 0.0766947790980339, 0.06249779462814331, -0.4374426305294037, -0.14102694392204285, 0.1524970531463623, -1.1072214841842651, -0.26568788290023804, 0.3533901870250702, -0.28557658195495605, -0.10240471363067627, -0.3848434090614319, 0.11502630263566971, 0.09993763267993927, 0.8748790621757507, 1.066508173942566, 0.056839585304260254, -0.9261329174041748}, {-0.0972515344619751, -0.2921789586544037, 0.26390892267227173, -0.22606252133846283, -0.49881672859191895, -0.12123030424118042, -0.07018715143203735, 0.5417667627334595, -0.24549679458141327, 0.9375701546669006, 0.05147188901901245, -0.15324607491493225, -0.026452623307704926, 1.1428825855255127, 0.009473763406276703, -0.027894403785467148, 0.9511917233467102, -0.303911417722702, -1.2095203399658203}, {0.13472622632980347, -0.1976025551557541, -0.20013977587223053, 0.22684139013290405, -0.9121308922767639, -0.3478614091873169, -0.16149863600730896, -0.7387512922286987, 0.24737727642059326, -1.2253706455230713, -0.1275368332862854, 0.2203618884086609, -0.4181967079639435, -1.5981261730194092, 0.39273640513420105, 1.5476207733154297, -1.0898520946502686, -0.30790096521377563, -0.4543204605579376}, {0.0, -0.06485496461391449, -0.10948494076728821, 0.0, -0.25583675503730774, -0.11512067914009094, 0.0, -0.13316956162452698, -0.07725024223327637, -0.12305805087089539, 0.0, -0.06628063321113586, -0.019621344283223152, -0.14825066924095154, -0.046479348093271255, -0.06731727719306946, -0.6088702082633972, 0.0, -0.6034234762191772}};

double OutputWeights[HiddenNodes + 1][OutputNodes] = {{0.3987823724746704, 0.25990307331085205, 0.27995365858078003, -0.4034218192100525, -0.3464948534965515, 0.45027244091033936, 0.11017441749572754, -0.3017178773880005, -0.3475404679775238}, {-0.2911466956138611, -0.21166670322418213, 0.36231401562690735, 0.062184035778045654, -0.2917046844959259, 0.06600368767976761, -0.35706570744514465, -0.32548844814300537, -0.3316330015659332}, {-0.044731710106134415, -0.36103829741477966, 0.35492122173309326, 0.23771554231643677, 0.11147463321685791, -0.3547144830226898, 0.2530961036682129, -0.43450838327407837, -0.3178069591522217}, {0.38510483503341675, -0.004979521036148071, -0.2049199640750885, 0.15250998735427856, -0.15205469727516174, 0.018308788537979126, -0.28238236904144287, -0.32411640882492065, -0.06797674298286438}, {-0.032038912177085876, 0.20946653187274933, 0.19711452722549438, 0.17459623515605927, 0.059888314455747604, -0.265841007232666, -1.0756195783615112, -1.0125569105148315, -1.3620233535766602}, {0.03235134482383728, 0.3546636998653412, 0.2715602517127991, 0.05988281965255737, 0.07245749235153198, 0.1390833556652069, 0.1206151694059372, -0.28985893726348877, 0.03131244331598282}, {0.16189861297607422, -0.012262582778930664, -0.227498397231102, 0.25400686264038086, 0.34491950273513794, 0.30097025632858276, -0.16854676604270935, 0.38530826568603516, -0.2406426966190338}, {-0.32186728715896606, 0.10224443674087524, -0.16942480206489563, 0.10571307688951492, 0.05608157068490982, 0.8434013724327087, -0.656745195388794, 0.4535583555698395, -1.501233458518982}, {-0.28397318720817566, 0.3747696280479431, -0.3414836525917053, -0.41579753160476685, 0.19191884994506836, -0.4226748049259186, 0.4414454400539398, 0.19707471132278442, 0.33207884430885315}, {-0.022243570536375046, -0.009119806811213493, -0.03199158236384392, -0.22967037558555603, -0.19519291818141937, 0.39950472116470337, -0.7189139127731323, -1.3411198854446411, -0.00014279774040915072}, {0.24908143281936646, -0.20174351334571838, 0.3649413585662842, 0.09305572509765625, -0.14556628465652466, -0.1227758526802063, 0.40461796522140503, 0.16137051582336426, -0.2901288866996765}, {0.4448866844177246, -0.34117600321769714, -0.4519665837287903, 0.0877382755279541, 0.13024067878723145, 0.06254317611455917, 0.4062882363796234, 0.31072014570236206, -0.41842690110206604}, {-0.06027141213417053, 0.4249926805496216, -0.25804129242897034, -0.17244544625282288, -0.4390818476676941, 0.1846015751361847, -0.34630224108695984, 0.36336612701416016, -0.3448646366596222}, {0.07499497383832932, -0.24474340677261353, -0.40777403116226196, -0.2754214406013489, -0.028247907757759094, -0.13137777149677277, 1.0028637647628784, -0.39219391345977783, 0.5885316729545593}, {-0.31400948762893677, 0.22454489767551422, 0.05370192602276802, -0.060241471976041794, 0.3228612542152405, -0.23467010259628296, -0.4348422884941101, -0.4560965895652771, 0.041073914617300034}, {0.25812679529190063, 0.2167736291885376, 0.02426958829164505, -0.008547528646886349, 0.14948749542236328, 0.200993150472641, 0.34658893942832947, 0.6557018160820007, -0.9849507212638855}, {0.11963637918233871, 0.11994536221027374, 0.18676547706127167, -0.3931336998939514, -0.34149613976478577, 0.12793312966823578, -0.16706031560897827, -0.2801564335823059, 0.9429376125335693}, {-0.32241421937942505, -0.37201035022735596, 0.0032708048820495605, -0.09051212668418884, -0.09737315773963928, -0.20744726061820984, 0.25834721326828003, -0.0781974196434021, -0.25734609365463257}, {-0.284027636051178, 0.22390435636043549, 0.3093751072883606, 0.25960639119148254, 0.15204980969429016, -0.07900215685367584, 1.124640941619873, -0.5463644862174988, -1.1648685932159424}, {-0.377765417098999, -0.49702906608581543, -0.3051963150501251, -0.25901371240615845, -0.24834343791007996, -0.42700427770614624, 0.0005310681881383061, 0.9999790191650391, 0.9997896552085876}};

//the function for which this whole circus is established
void InputToOutput(double TestInput[])
{
  /******************************************************************
    Compute hidden layer activations
  ******************************************************************/

  for ( i = 0 ; i < HiddenNodes ; i++ ) 
  {
    Accum = HiddenWeights[InputNodes][i];
    for ( j = 0 ; j < InputNodes ; j++ ) 
    {
      Accum += TestInput[j] * HiddenWeights[j][i];
    }
    Hidden[i] = (Accum>0?Accum:0);
  }

  /******************************************************************
    Compute output layer activations and calculate errors
  ******************************************************************/

  for ( i = 0 ; i < OutputNodes ; i++ ) 
  {
    Accum = OutputWeights[HiddenNodes][i];
    for ( j = 0 ; j < HiddenNodes ; j++ ) 
    {
      Accum += Hidden[j] * OutputWeights[j][i];
    }
    //linear activation function
    Output[i] = (Accum>0?Accum:0);
  }

  std::cout<<"Predicted Output: ";
  for ( i = 0 ; i < OutputNodes ; i++ ) 
  {
    std::cout<<Output[i]<<" ";
  }
  std::cout<<"\n";

}

int main() 
{
  /* double input[InputNodes];
  unsigned whichDataSet;
  std::cout<<"Choose the dataset: ";
  std::cin>>whichDataSet;
  for(i = 0; i<InputNodes; i++)
  {
    input[i] = Input[whichDataSet][i];
  } */

  unsigned inputIndex;
  std::cout<<"Enter input index\n";
  std::cin>>inputIndex;
  
  InputToOutput(Input[inputIndex]);
/*   std::cout<<"Actual Output: ";
  for(i = 0; i<OutputNodes; i++)
  {
    std::cout<<Target[whichDataSet][i]<<" ";
  }
  std::cout<<"\n"; */
  return 0;
}

//other weights

//Wild oscillation data
// {{-2.4062089920043945, 0.9986947178840637, -1.0671565532684326, -1.3078217506408691, 2.282473087310791}, {0.023553701117634773, 4.477676868438721, 0.913195788860321, 5.250211715698242, -1.712435245513916}, {3.6402392387390137, -4.424204349517822, 0.7046148777008057, -3.124873638153076, 0.3226388394832611}, {-0.9930652379989624, 0.6067217588424683, -2.885045051574707, 1.894884467124939, -3.402651071548462}, {1.8534878492355347, -0.76656174659729, 0.03851019963622093, -3.167191505432129, -1.8511136770248413}};
//{{-2.664714813232422, 1.3368545770645142}, {-2.16292142868042, -6.365833759307861}, {-0.7271573543548584, -2.766731023788452}, {3.6028904914855957, 5.587453842163086}, {-3.2445333003997803, 1.2343003749847412}, {3.666773557662964, 1.8885525465011597}};
