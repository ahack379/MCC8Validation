tot_bnbcos = 101600. 
tot_off = 377273. 
tot_on = 341100.
tot_intime = 25874

scale_intime = 10.279 
scale_onoffbeam = 1.23 * (382718./tot_off) * (tot_on/547616.) ;

int_per_1e20POT = 99035.2  
dataPOT = 0.495 * (tot_on) / 547616 
mcbnbcos_POT = float(tot_bnbcos)/int_per_1e20POT 

print "Data POT : ", dataPOT
print "MCBNB POT : ",mcbnbcos_POT

print "To scale MCBNBCos to onbeam, multiply mcbnbcos by : ",dataPOT/mcbnbcos_POT
print "To scale off beam to onbeam, mult off beam by ", scale_onoffbeam
print "To scale InTime, mult intime by: ",dataPOT/mcbnbcos_POT*scale_intime * tot_bnbcos/tot_intime
