Postup přeložení a spuštění testů
-----------------------
- Stáhnout si google test repozitář přes submodule - příkaz `git submodule update --init`
- Spustit make test

Přidání vlastního testu
------------------------
- Do CMakeLists.txt v rootu přidat do Headers a Sources případné nové zdrojové soubory
- Do CMakeLists.txt v adresáři tests zkopírovat trojici příkazů `add_executable`, `target_link_libraries` a `add_test` a přepsat hodnoty pro svůj test
- Do makefile do TESTS přidat cestu k .cpp souboru nového testu a do test: přidat do zřetězených příkazu spuštění testu
- Spustit make test

