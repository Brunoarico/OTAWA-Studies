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
#define ELF_TEST "../din/"
#define ELF_OTAWA_FOLDER "../din/build/main_otawa.elf"



// test if files are compiled
TEST(CompileTest, CompileTest)
{
   int result = false;
   compile();
   FILE *fa = fopen(ELF_TEST".merge.cm", "r");
   FILE *fb = fopen(ELF_TEST".rec_status", "r");

   FILE *fc = fopen(ELF_TEST"build/main_empty.ff", "r");
   FILE *fd = fopen(ELF_TEST"build/main_otawa.elf", "r");
   FILE *fe = fopen(ELF_TEST"build/main.bin", "r");
   FILE *ff = fopen(ELF_TEST"build/main.bin.lst", "r");
   FILE *fg = fopen(ELF_TEST"build/main.elf", "r");
   FILE *fh = fopen(ELF_TEST"build/main.xml", "r");
   if (fa && fb && fc && fd && fe && ff && fg && fh)
   {
     result = true;
   }
   ASSERT_TRUE(result);
}
// Generate .ff and .elf files for OTAWA
TEST(GenffTest, GenFF)
{
   genff();
   FILE *f = fopen(ELF_FOLDER, "r");
   ASSERT_TRUE(f != NULL);
}




/*
 * Verify if WCET IPET functionality is working
 * 
 */
TEST(WcetIipetM3Test, WcetIpetcalc)
{
   uint32_t wcet_it = -1;
   cfgGen otawaInstance("m3", "main", ELF_OTAWA_FOLDER);
   WCETCalculator wcetIpet(otawaInstance.workspaceGenerator());
   wcetIpet.calculateWCET();
   wcet_it = wcetIpet.getWCET();
   ASSERT_TRUE(wcet_it);
}


/*
 * Verify if WCET Bio functionality is working
 * 
 */

TEST(WcetBioM3Test, WcetBiocalc)
{
   otawa::WorkSpace *ws;
   uint32_t wcet_b = -1;
   cfgGen otawaInstance("m3", "main", ELF_OTAWA_FOLDER);
   otawaInstance.workspaceGenerator();
   WCETCalculatorBio wcetBio(otawaInstance.cfg2Matrix());
   wcetBio.calculateWCET();
   wcet_b = wcetBio.getWCET();
   ASSERT_TRUE(wcet_b);
}

// TEST(ACOCalculate, ACOCalculate)
// {
//    otawa::WorkSpace *ws;
//    uint32_t wcet_b = -1;
//    cfgGen otawaInstance("m3", "main", ELF_OTAWA_FOLDER);
//    otawaInstance.workspaceGenerator();
//    WCETCalculatorBio wcetBio(otawaInstance.cfg2Matrix());
//    std::set<CfgMatrix> mySet = otawaInstance.cfg2Matrix();
//     for (const CfgMatrix& elem : mySet) pq.push(elem);
    
//     while (!pq.empty()) {
//         CfgMatrix c = pq.front();
//         pq.pop();
        
//         int maxIter = 30;
//         int antNo = 10;
//         float rho = 0.8;
//         if (c.getPriority() > 0 && !replaceDependencies(&c)) {
//             pq.push(c);
//         } else {
//             c.printCycles();
//             ACO aco(c, antNo, 0, maxIter, alpha, beta, rho);
//             aco.simulate();
//             wcet = aco.getResults();
//             cfgMap[c.getMyHashName()] = wcet;
//         }
//     }
//    ASSERT_TRUE(wcet_b);
// }

/*
 * Verify if OTAWA workspace generator is working
 * 
 */
TEST(WorkSpaceGeneratorTest, WorkSpace)
{
   otawa::WorkSpace *ws;
   uint32_t wcet_b = -1;
   cfgGen otawaInstance("m3", "main", ELF_OTAWA_FOLDER);
   ASSERT_TRUE(otawaInstance.workspaceGenerator());
}


/*
 * Verify if cfg2Matrix function returns a cfgmatrix data type
 * 
 */
TEST(CfgToMatrixTest, CfgMatrixTest)
{
   otawa::WorkSpace *ws;
   cfgGen otawaInstance("m3", "main", ELF_OTAWA_FOLDER);
   otawaInstance.workspaceGenerator();
   std::set<CfgMatrix> mySet = otawaInstance.cfg2Matrix();
   for (const auto& cfgMatrix : mySet) {
      ASSERT_TRUE(dynamic_cast<const CfgMatrix*>(&cfgMatrix) != nullptr);
   }
  
}

TEST(WcetDynamicTest, WcetDynamicCalc)
{
   // uint32_t wcet_d = -1;
   // WCETCalculatorDyn wcetDyn(DEBUG_FILE, ELF_FOLDER);
   // wcetDyn.calculateWCET();
   // wcet_d = wcetDyn.getWCET();
   // std::cout << wcet_d << "\n";
   // ASSERT_TRUE(wcet_d);
}

// int main(int argc, char **argv)
// {
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
// }
