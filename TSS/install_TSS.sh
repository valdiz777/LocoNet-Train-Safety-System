#/bin/bash
#################################
# Author: Valentine C. Nwachukwu
# Email: valdiz777@gmail.com
# DESC: This is a TSS setup script for Debian Jessie

############### Pre-Reqs ########
echo "Installing Pre-Reqs"
echo "==================="
sudo apt-get install -y build-essential

############## Install Qt5Defaults #########
echo "Installing Qt5Defaults"
echo "========================="
sudo apt-get install -y qt5-default
cd TSS
tss_dir = "$PWD"

chmod +x startTSS.sh
chmod +x startTSS.bash
qmake TSS.pro
make
echo "Done installing TSS: Train Safety System..."
echo "Creating Alias for TSS"
echo "======================"
echo "alias TSS \"cd $tss_dir/build/debug && ./../../startTSS.sh\"" >> ~/.bashrc
echo "alias TSS \"cd $tss_dir/build/debug && ./../../startTSS.sh\"" >> ~/.profile
source ~/.bashrc
source ~/.profile
echo "All systems are a go. type TSS to begin..."
