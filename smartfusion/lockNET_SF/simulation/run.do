quietly set ACTELLIBNAME SmartFusion
quietly set PROJECT_DIR "N:/Git/locknet373/smartfusion/lockNET_SF"

if {[file exists presynth/_info]} {
   echo "INFO: Simulation library presynth already exists"
} else {
   file delete -force presynth 
   vlib presynth
}
vmap presynth presynth
vmap smartfusion "C:/Microsemi/Libero_SoC_v11.8/Designer/lib/modelsim/precompiled/vlog/smartfusion"
vmap MSS_BFM_LIB "../component/Actel/SmartFusionMSS/MSS/2.5.200/mti/user_verilog/MSS_BFM_LIB"
vcom -work MSS_BFM_LIB -force_refresh
vlog -work MSS_BFM_LIB -force_refresh
if {[file exists COREAPB3_LIB/_info]} {
   echo "INFO: Simulation library COREAPB3_LIB already exists"
} else {
   file delete -force COREAPB3_LIB 
   vlib COREAPB3_LIB
}
vmap COREAPB3_LIB "COREAPB3_LIB"

vlog -vlog01compat -work presynth "${PROJECT_DIR}/smartgen/RSA_64b_RAM/RSA_64b_RAM.v"
vlog -vlog01compat -work presynth "${PROJECT_DIR}/hdl/MonMult.v"
vlog -vlog01compat -work presynth "${PROJECT_DIR}/hdl/RSA.v"
vlog "+incdir+${PROJECT_DIR}/hdl" "+incdir+${PROJECT_DIR}/stimulus" -vlog01compat -work presynth "${PROJECT_DIR}/stimulus/rsa_ram_tb.v"

vsim -L smartfusion -L presynth -L MSS_BFM_LIB -L COREAPB3_LIB  -t 1ps presynth.rsa_ram_tb
add wave /rsa_ram_tb/*
run 1000ns
