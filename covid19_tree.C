//
// covid19_tree()
// (c) 2020 Peter Steinberg, all errors my own
//
// A simple utility to convert the CSV file available from https://covidtracking.com/data
// to a format amenable to folks who use ROOT for data analysis.
//
// The CSV list of fields, from data file but stripped in ROOT macro
// date,state,positive,negative,pending,hospitalizedCurrently,hospitalizedCumulative,inIcuCurrently,inIcuCumulative,onVentilatorCurrently,onVentilatorCumulative,recovered,hash,dateChecked,death,hospitalized,total,totalTestResults,posNeg,fips,deathIncrease,hospitalizedIncrease,negativeIncrease,positiveIncrease,totalTestResultsIncrease
//
// An example data line
// 20200403,AK,157,5859,,,15,,,,,,192f851a8f9b68576ed308814322d1333b538699,2020-04-03T20:00:00Z,3,15,6016,6016,6016,02,0,6,980,14,994
//
// Enjoy - questions to steinber@gmail.com
//

TTree* cv19;

void download_data()
{
  // original is daily.csv
  // version w/ no titles is daily2.csv
  // version w/ padded 0's is daily3.csv --> this was the most important, since ROOT otherwise ignores empty fields.
  
  gSystem->Exec("curl -O https://covidtracking.com/api/v1/states/daily.csv");
  gSystem->Exec("tail -n +2 daily.csv > daily2.csv");
  gSystem->Exec("awk -F, -v OFS=\',\' \'{for(i=1;i<=NF;i++) if(!$i)$i=\"0\"}1\' daily2.csv > daily3.csv");

}

int dayinyear(int date)
{
  int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  int year = date/10000;
  if (year%4==0) days[1] = 29; // leap year
  int month = int((date%10000)/100);
  int day = date%100;
  int d = 0;
  for (int i=0;i<month-1;i++) {d+=days[i];}
  d += (day-1);
  return d;
}

TGraph* make_graph(TString state, TString variable)
{
  TString var = variable;
  var += ":dayinyear(date)";
  
  TString cut = "state==\"";
  cut += state;
  cut += "\"";
  
  int n = cv19->Draw(var,cut,"goff");
  TGraph* g = new TGraph(n,cv19->GetV2(),cv19->GetV1()); // ROOT flips the axes for typical Draw's.
  return g;
}

TTree* covid19_tree(bool get_data = 1)
{
  if (get_data) download_data();
  
  cv19 = new TTree("cv19","COVID19 USA");

  cv19->ReadFile("daily3.csv","date/I:state/C:positive/I:negative/I:pending/I:hospitalizedCurrently/I:hospitalizedCumulative/I:inIcuCurrently/I:inIcuCumulative/I:onVentilatorCurrently/I:onVentilatorCumulative/I:recovered/I:hash/C:dateChecked/C:death/I:hospitalized/I:total/I:totalTestResults/I:posNeg/I:fips/I:deathIncrease/I:hospitalizedIncrease/I:hospitalizedIncrease/I:positiveIncrease/I:totalTestResultsIncrease/I");

  return cv19;
}
    
