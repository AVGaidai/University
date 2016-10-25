#!/bin/bash

clear

ssh gaidai@jet.cpct.sibsutis.ru "./DCS/lab_01/print_state_dcs.sh"
scp gaidai@jet.cpct.sibsutis.ru:~/DCS/lab_01/.pbsnodes_info ./

cat .pbsnodes_info


