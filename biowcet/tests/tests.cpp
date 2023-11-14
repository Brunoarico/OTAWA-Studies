#include <gtest/gtest.h>
#include "aux_functions.cpp"
#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include "../src/cfgGen/cfgGen.h"
#include "../src/wipet/wcet_ipet.h"
#include "../src/wbio/wcet_bio.h"
#include "../src/wdyn/wcet_dyn.h"
#include "../src/aco/aco.h"

#define TEST_FILE "../test_files/if/main.c "
#define ARCH "m3"
#define TARGET "main"

#define DIN_FOLDER "../din/"
#define SRC_FOLDER DIN_FOLDER "src/main.c"
#define BLD_FOLDER "/build/"
#define DEBUG_FILE "/hwdebug.py"
#define ELF_FILE BLD_FOLDER "/main.elf"
#define ELF_OTAWA_FILE DIN_FOLDER BLD_FOLDER "main_otawa.elf"
#define FF_FILE DIN_FOLDER BLD_FOLDER "/main_empty.ff"
#define BINARY_FILE DIN_FOLDER BLD_FOLDER "/main.bin"
#define XML_FILE DIN_FOLDER BLD_FOLDER "/main.xml"

TEST(CopyCDirTest, CopyCDir)
{
   executeAndLog(" cp " TEST_FILE DIN_FOLDER);
   FILE *f = fopen(SRC_FOLDER, "r");
   ASSERT_TRUE(f != NULL);
}

// test if files are compiled
TEST(CompileTest, CompileTest)
{
   int result = false;
   compile();
   FILE *fc = fopen(FF_FILE, "r");
   FILE *fd = fopen(ELF_OTAWA_FILE, "r");
   FILE *fe = fopen(BINARY_FILE, "r");
   FILE *fg = fopen(DIN_FOLDER ELF_FILE, "r");
   FILE *fh = fopen(XML_FILE, "r");
   if (fc && fd && fe  && fg && fh) {
      result = true;
   }
   ASSERT_TRUE(result);
}

// Generate .ff and .elf files for OTAWA
TEST(GenffTest, GenFF)
{
   genff();
   FILE *f = fopen(DIN_FOLDER ELF_FILE, "r");
   ASSERT_TRUE(f != NULL);
}



TEST(CopyDirTest, CopyDir)
{
   int result = false;
   compile();
   FILE *fc = fopen(FF_FILE, "r");
   FILE *fd = fopen(ELF_OTAWA_FILE, "r");
   FILE *fe = fopen(BINARY_FILE, "r");
   FILE *fg = fopen(DIN_FOLDER ELF_FILE, "r");
   FILE *fh = fopen(XML_FILE, "r");
   if (fc && fd && fe && fg && fh)
   {
      result = true;
   }
   ASSERT_TRUE(result);
}

/*
 * Verify if WCET IPET functionality is working
 *
 */
TEST(WcetIipetM3Test, WcetIpetcalc)
{
   uint32_t wcet_it = -1;
   cfgGen otawaInstance(ARCH, TARGET, ELF_OTAWA_FILE);
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
   cfgGen otawaInstance(ARCH, TARGET, ELF_OTAWA_FILE);
   otawaInstance.workspaceGenerator();
   WCETCalculatorBio wcetBio(otawaInstance.cfg2Matrix());
   wcetBio.calculateWCET();
   wcet_b = wcetBio.getWCET();
   ASSERT_TRUE(wcet_b);
}

/*
 * Verify if WCET Bio functionality is working
 *
 */
TEST(ACOSimulateTest, ACOSimulate)
{
   otawa::WorkSpace *ws;
   std::queue<CfgMatrix> pq;
   uint32_t wcet = -1;
   double alpha = 1; // Phromone exponential parameters
   double beta = 1;  // Desirability exponential parameter
   int maxIter = 30;
   int antNo = 10;
   float rho = 0.8;
   cfgGen otawaInstance(ARCH, TARGET, ELF_OTAWA_FILE);
   otawaInstance.workspaceGenerator();
   std::set<CfgMatrix> mySet = otawaInstance.cfg2Matrix();
   for (const CfgMatrix &elem : mySet)
      pq.push(elem);

   while (!pq.empty())
   {
      CfgMatrix c = pq.front();
      pq.pop();

      int maxIter = 30;
      int antNo = 10;
      float rho = 0.8;

      ACO aco(c, antNo, 0, maxIter, alpha, beta, rho);
      aco.simulate();
      wcet = aco.getResults();
      std::cout << wcet << "\n";
      // cfgMap[c.getMyHashName()] = wcet;
   }
   ASSERT_TRUE(wcet);
}

TEST(CFGPrintCyclesTest, CFGPrintCycles)
{
   otawa::WorkSpace *ws;
   std::queue<CfgMatrix> pq;
   uint32_t wcet = -1;
   double alpha = 1; // Phromone exponential parameters
   double beta = 1;  // Desirability exponential parameter
   int maxIter = 30;
   int antNo = 10;
   float rho = 0.9;
   setVerbose(true);
   cfgGen otawaInstance(ARCH, TARGET, ELF_OTAWA_FILE);
   otawaInstance.workspaceGenerator();
   std::set<CfgMatrix> mySet = otawaInstance.cfg2Matrix();
   for (const CfgMatrix &elem : mySet)
      pq.push(elem);

   while (!pq.empty())
   {
      CfgMatrix c = pq.front();
      pq.pop();

      int maxIter = 30;
      int antNo = 10;
      float rho = 0.8;
      c.printCycles();
      ACO aco(c, antNo, 0, maxIter, alpha, beta, rho);
      aco.simulate();
      wcet = aco.getResults();
   }
   ASSERT_TRUE(wcet);
}

/*
 * Verify if OTAWA workspace generator is working
 *
 */
TEST(WorkSpaceGeneratorTest, WorkSpace)
{
   otawa::WorkSpace *ws;
   cfgGen otawaInstance(ARCH, TARGET, ELF_OTAWA_FILE);
   ASSERT_TRUE(otawaInstance.workspaceGenerator());
}

/*
 * Verify if cfg2Matrix function returns a cfgmatrix data type
 *
 */
TEST(CfgToMatrixTest, CfgMatrixTest)
{
   otawa::WorkSpace *ws;
   cfgGen otawaInstance(ARCH, TARGET, ELF_OTAWA_FILE);
   otawaInstance.workspaceGenerator();
   std::set<CfgMatrix> mySet = otawaInstance.cfg2Matrix();
   for (const auto &cfgMatrix : mySet)
   {
      ASSERT_TRUE(dynamic_cast<const CfgMatrix *>(&cfgMatrix) != nullptr);
   }
}

TEST(WriteOutputTest, WriteOutput)
{
   int result = false;
   uint32_t wceti = 40;
   uint32_t wcetb = 43;
   uint32_t wcetd = 44;
   bool dyn = true;
   toFile("../", TARGET, wceti, wcetb, wcetd, dyn);
   FILE *f = fopen("./results.txt", "r");
   ASSERT_TRUE(f != NULL);
}

TEST(WcetDynamicTest, WcetDynamicCalc)
{
   uint32_t wcet_d = -1;
   WCETCalculatorDyn wcetDyn(DEBUG_FILE, ELF_FILE);
   wcetDyn.calculateWCET();
   wcet_d = wcetDyn.getWCET();
   std::cout << wcet_d << "\n";
   ASSERT_TRUE(wcet_d);
}

// int main(int argc, char **argv)
// {
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
// }
