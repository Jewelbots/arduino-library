echo -e "commit and push to master";
    git add .
    git commit -am "$1";
    git push origin master;
echo -e "zip up folder then move out of folder";
    tar cvzf jewelbots_nRF51822_board_v100_new.tar.gz jewelbots_nRF51822_board_v100;
    cp jewelbots_nRF51822_board_v100_new.tar.gz ../jewelbots_nRF51822_board_v100.tar.gz;
    rm jewelbots_nRF51822_board_v100_new.tar.gz;
echo -e "checkout gh-pages then copy folder back";
    git checkout gh-pages;
    git pull --rebase origin gh-pages;
    rm jewelbots_nRF51822_board_v100.tar.gz;
    cp ../jewelbots_nRF51822_board_v100.tar.gz jewelbots_nrf51822_board_v100.tar.gz
    rm ../jewelbots_nRF51822_board_v100.tar.gz
echo -e "overwrite local folder then push to gh pages";
    git add .;
    git commit -am "$1";
    git push origin gh-pages;
    rm jewelbots_nRF51822_board_v100.tar.gz
echo -e "checkout master and go! ";
    git checkout master;
