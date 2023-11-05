#include <gtest/gtest.h>
#include "aux_functions.cpp"
#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include "../src/cfgGen/cfgGen.h"
#include "../src/wipet/wcet_ipet.h"
#include "../src/wbio/wcet_bio.h"
#include "../src/wdyn/wcet_dyn.h"


#define DEBUG_FILE "../din/hwdebug.py"
#define ELF_FOLDER "../din/build/main.elf"
#define ELF_OTAWA_FOLDER "../din/build/main_otawa.elf"


// testes responsável por gerar o arquivo  .elf e .ff de testes do OTAWA
TEST(GenffTest, GenFF)
{
   genff();
   FILE *f = fopen(ELF_FOLDER, "r");
   ASSERT_TRUE(f != NULL);
}

/*
 * Teste responsável por calcular o WCET pelo método IPET utilizando a arquitetura padrão
 * Caso haja erro no cálculo, o valor da variável wcet_it é negativo e o teste terá falhadao
 */

TEST(WcetIipetTrivialTest, WcetIpetcalc)
{
   uint32_t wcet_it = -1;
   cfgGen otawaInstance("trivial", "main", ELF_OTAWA_FOLDER);
   WCETCalculator wcetIpet(otawaInstance.workspaceGenerator());
   wcetIpet.calculateWCET();
   wcet_it = wcetIpet.getWCET();
   std::cout << wcet_it << "\n";
   ASSERT_TRUE(wcet_it);
}

/*
 * Teste responsável por calcular o WCET pelo método IPET utilizando a arquitetura xmc4500
 * Caso haja erro no cálculo, o valor da variável wcet_it é negativo e o teste terá falhadao
 */

TEST(WcetIipetXmcTest, WcetIpetXmccalc)
{
   uint32_t wcet_it = -1;
   cfgGen otawaInstance("trivial", "main", ELF_OTAWA_FOLDER);
   WCETCalculator wcetIpet(otawaInstance.workspaceGenerator());
   wcetIpet.calculateWCET();
   wcet_it = wcetIpet.getWCET();
   std::cout << wcet_it << "\n";
   ASSERT_TRUE(wcet_it);
}

/*
 * Teste responsável por calcular o WCET pelo método BioInspirado utilizando a arquitetura padrão
 * Caso haja erro no cálculo, o valor da variável wcet_it é negativo e o teste terá falhadao
 */

// TEST(WcetBioTest, WcetBiocalc)
// {
//    otawa::WorkSpace *ws;
//    uint32_t wcet_b = -1;
//    cfgGen otawaInstance("trivial", "main", "/root/OTAWA-Studies/biowcet/din/build/main_otawa.elf");
//    WCETCalculatorBio wcetBio(otawaInstance.cfg2Matrix());
//    wcetBio.calculateWCET();
//    wcet_b = wcetBio.getWCET();
//    // std::cout << wcet_b << "\n";
//    ASSERT_TRUE(wcet_b);
// }

/*
 * Teste responsável por calcular o WCET pelo método BioInspirado utilizando a arquitetura xmc4500
 * Caso haja erro no cálculo, o valor da variável wcet_it é negativo e o teste terá falhadao
 */

TEST(WcetBioXmcTest, WcetBioXmccalc)
{
   otawa::WorkSpace *ws;
   uint32_t wcet_b = -1;
   cfgGen otawaInstance("xmc4500", "main", ELF_OTAWA_FOLDER);
   otawaInstance.workspaceGenerator();
   WCETCalculatorBio wcetBio(otawaInstance.cfg2Matrix());
   wcetBio.calculateWCET();
   wcet_b = wcetBio.getWCET();
   std::cout << wcet_b << "\n";
   ASSERT_TRUE(wcet_b);
}

// TEST(WcetDynamicTest, WcetDynamicCalc)
// {
//    // uint32_t wcet_d = -1;
//    // WCETCalculatorDyn wcetDyn(DEBUG_FILE, ELF_FOLDER);
//    // wcetDyn.calculateWCET();
//    // wcet_d = wcetDyn.getWCET();
//    // std::cout << wcet_d << "\n";
//    // ASSERT_TRUE(wcet_d);
// }

int main(int argc, char **argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
