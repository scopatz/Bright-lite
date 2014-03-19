#include "parser.h"

using namespace std;

double IssToDouble(istringstream &iss){
    double num1;
    iss >> num1;
    return (num1);
}

isoInformation ParseOriginFile(string file_location){
    string tape = "/TAPE6";
    ifstream inf("home/Robert/Bright-lite/U-235/TAPE6.out");
    ofstream outf;
    outf.open("C:/Users/Robert/Documents/TAPE61.out");
    // Read error message.
    if (!inf){
        cerr << "Could not read file " + tape + " in " + file_location;
    }
    if (!outf){
        cerr << "Could not write file\n";
    }
    isoInformation iso_info;
    string line;
    while (getline(inf, line)) {
        if (line.find("NEUT PRODN") == 0){
            istringstream iss(line);
            string test;
            iss >> test >> test;
            double value;
            while(iss >> value){
                iso_info.neutron_prod.push_back(value);
            }
        }
        if (line.find("NEUT DESTN") == 0){
            istringstream iss(line);
            string test;
            iss >> test >> test;
            double value;
            while (iss >> value){
                iso_info.neutron_dest.push_back(value);
            }
        }
        if (line.find("K INFINITY") == 0){
            istringstream iss(line);
            string test;
            iss >> test >> test;
            double value;
            while (iss >> value){
                iso_info.k_inf.push_back(value);
            }
        }
        if (line.find("BURNUP,MWD") == 0){
            istringstream iss(line);
            string test;
            iss >> test;
            double value;
            while (iss >> value){
                iso_info.BUd.push_back(value);
            }
        }
        if(line.find("REACTIVITY AND BURNUP DATA") == 45){
            getline(inf, line);
            getline(inf, line);
            getline(inf, line);
            istringstream iss(line);
            double time_pass;
            string day;
            while(iss >> time_pass >> day){
                iso_info.time.push_back(time_pass);
            }
        }
        if(line.find("5 SUMMARY TABLE:  C") == 21){
            getline(inf, line);
            getline(inf, line);
            double num1;
            double num2;
            string expon;
            int j = 0;
            bool test = false;
            while( getline(inf, line)){
                if(line.find("SUMTOT") == 0 || j == 51) {
                    break;
                } else {
                    j += 1;
                    string iso_name = line.substr(0, 11);
                    iso_name.erase(remove(iso_name.begin(), iso_name.end(), ' '), iso_name.end());
                    if(iso_name.find_first_of("0123456789") != std::string::npos){
                        for (int i = 0; i < iso_info.iso_vector.size(); i++){
                            if (iso_info.iso_vector[i].name == iso_name){
                                test = true;
                                for ( int j = 11; j < 130 ; j += 10){
                                    istringstream iss(line.substr(j,9));
                                    iso_info.iso_vector[i].mass.push_back(IssToDouble(iss));
                                }
                            }
                        }
                        if (test == false){
                            daughter daughter;
                            daughter.name = iso_name;
                            for ( int j = 11; j < 130 ; j += 10){
                                istringstream iss(line.substr(j,9));
                                daughter.mass.push_back(IssToDouble(iss));
                            }
                            iso_info.iso_vector.push_back(daughter);
                        }
                    }
                }
            }
        }
    }
    inf.close();
    outf << "TIME" << "    ";
    for(int i = 0; i < iso_info.time.size(); i++){
        outf << iso_info.time[i] << "   ";
    }
    outf << "\n" << "NEUT_PROD" << "    ";
    for(int i = 0; i < iso_info.neutron_prod.size(); i++){
        outf << iso_info.neutron_prod[i] << "   ";
    }
    outf << "\n" << "NEUT_DEST" << "    ";
    for(int i = 0; i < iso_info.neutron_dest.size(); i++){
        outf << iso_info.neutron_dest[i] << "   ";
    }
    outf << "\n" << "BUd" << "    ";
    for(int i = 0; i < iso_info.BUd.size(); i++){
        outf << iso_info.BUd[i] << "    ";
    }
    outf << "\n" << "";
    for (int i = 0; i < iso_info.iso_vector.size(); i++){
        if (iso_info.iso_vector[i].mass[11] > 0.01){
            outf << iso_info.iso_vector[i].name << "    ";
            for (int j = 0; j < iso_info.iso_vector[i].mass.size(); j++){

                outf << iso_info.iso_vector[i].mass[j] << "    ";
            }
            outf << "\n";
        }
    }
    outf.close();
    return iso_info;
}



int main(){
    isoInformation testVector;
   // testVector = ParseOriginFile("C:/brightlite");
    double BUd_sum = 0;
    int N;
    double X;
    isoInformation test1;

    double BU_end;

    int ip;
    cout << "1. Enrichment to Burnup" << endl << "2. Burnup to Enrichment" << endl;

    cin >> ip;

    switch (ip)
    {
    case 1:
        cout << "Enter enrichment in percent: ";
        cin >> X;
        X = X/100.0;
        cout << "Enter number of batches: ";
        cin >> N;
        cout << "Burnup is  " << burnupcalc(DataReader(test1,X), N, .01).first << endl << endl ;
        break;
    case 2:
        cout << "Enter desired Burnup (0-200): ";
        cin >> BU_end;
        cout << "Enter number of batches: ";
        cin >> N;
        cout << "Desired enrichment is about:  " << enrichcalc(BU_end, N, 1)*100 << " %" << endl << endl;
        break;
    default:
        cout << endl<< "yeaah, no" << endl << endl;

    }
  return 0;
}


















