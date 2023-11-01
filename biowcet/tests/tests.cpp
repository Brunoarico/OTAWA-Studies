#include <gtest/gtest.h>
#include "gen_ff_function.h"
#include "/home/OTAWA-Studies/biowcet/src/wipet/wcet_ipet.h"
#include "/home/OTAWA-Studies/biowcet/src/wbio/wcet_bio.h"
#include "/home/OTAWA-Studies/biowcet/src/wdyn/wcet_dyn.h"

#define DEBUG_FILE "/hwdebug.py"
#define ELF_FOLDER "/build/main.elf"
// #include "/src/wipet/wcet_ipet.h"

// Include the necessary headers and dependencies

TEST(GenffTest, GenFF)
{
   genff();

   FILE *f = fopen("/home/OTAWA-Studies/biowcet/din/build/main.elf", "r");

   ASSERT_TRUE(f != NULL);
}

TEST(WcetIipetTrivialTest, WcetIpetcalc)
{
   uint32_t wcet_it = -1;
   WCETCalculator wcetIpet("trivial", "main", "/root/OTAWA-Studies/biowcet/din/build/main_otawa.elf");
   wcetIpet.calculateWCET();
   wcet_it = wcetIpet.getWCET();

   ASSERT_TRUE(wcet_it);
}

TEST(WcetBioTest, WcetBiocalc)
{
   uint32_t wcet_b = -1;
   WCETCalculatorBio wcetBio("trivial", "main", "/root/OTAWA-Studies/biowcet/din/build/main_otawa.elf");
   wcetBio.calculateWCET();
   wcet_b = wcetBio.getWCET();
   // std::cout << wcet_b << "\n";
   ASSERT_TRUE(wcet_b);
}

// TEST(WcetDynamicTest, WcetDynamicCalc)
// {
//       uint32_t wcet_d = -1;
//    WCETCalculatorDyn wcetDyn(DEBUG_FILE, ELF_FOLDER);
//    wcetDyn.calculateWCET();
//    wcet_d = wcetDyn.getWCET();
//    // std::cout << wcet_b << "\n";
//    ASSERT_TRUE(wcet_d);
// }

int main(int argc, char **argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
