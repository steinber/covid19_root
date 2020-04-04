# covid19_root
This is a set of simple (for now) ROOT macros to utilize public COVID19 data.

The dataset is from @covidtracking

Example:
<pre>
root [0] .L covid19_tree.C (load the macro)
root [1] TTree* cv19 = covid19_tree() (download the data and create the tree - US only - from COVID19 tracking)
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  193k  100  193k    0     0   726k      0 --:--:-- --:--:-- --:--:--  726k
(TTree *) 0x7ffc2478d8e0
root [2] cv19->Draw("hospitalized:dayinyear(date)","state==\"FL\"")
</pre>
