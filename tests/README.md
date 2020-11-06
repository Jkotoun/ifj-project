Postup přeložení testů
-----------------------
- Stáhnout si google test repozitář přes submodule - příkaz `git submodule update --init`
- Z adresáře build spustit spustit příkaz `cmake ..'
- Pokud testy chcete debugovat, ze složky build je třeba spustit `cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug ..`
- Testy se zkompilují příkazem 'make' ve složce build nebo build/tests

Přidání vlastního testu
------------------------
- Do CMakeLists.txt v rootu přidat do Headers a Sources případné nové zdrojové soubory
- Do CMakeLists.txt v adresáři tests zkopírovat trojici příkazů `add_executable`, `target_link_libraries` a `add_test` a přepsat hodnoty pro svůj test
- Znovu spustit `cmake ..` z buildu

