#include <iostream>
#include <gtkmm.h>

#include "parser/Parser.hpp"
#include "parser/FileReader.h"
#include "Global.h"
#include "ui/widgets.h"
#include "ErrHelper.h"

using namespace llvm;
Lexer *m_lexer;

static void activate(GtkApplication *app, gpointer data);

int startAnalyze(const char *path);

int main(int argc, char **argv) {

#ifdef CGUI
    auto app = Gtk::Application::create(argc, argv, APPNAME, Gio::APPLICATION_FLAGS_NONE);
    auto builder = Gtk::Builder::create_from_file("../ui/ui/main.glade");
    Gtk::Window *window = NIL;
    builder->get_widget("main_window", window);
    if (window == NIL) {
        LogError("main_window 未找到");
        return -1;
    }
    window->set_size_request(1200,800);
    return app->run(*window);
#else
    return startAnalyze("../test/test.c");
#endif
}

int startAnalyze(const char *path) {
    // Open a new module.
    TheModule = std::make_unique<Module>("Kingtous' jit", TheContext);
//    TheModule->setDataLayout(TheJIT->getTargetMachine().createDataLayout());
//     Do simple "peephole" optimizations and bit-twiddling optzns.
    // Reassociate expressions.

//    TheFPM->addPass(createReassociatePass());
//    // Eliminate Common SubExpressions.
//    TheFPM->addPass(createGVNPass());
//    // Simplify the control flow graph (deleting unreachable blocks, etc).
////    auto pass = createGVNPass();
//    TheFPM->addPass(createGVNPass());
    //
    FileReader reader(path);
//    FileReader reader("../test/case/section1/functional_test/12_array_traverse.sy");
    m_lexer = new Lexer(reader);
    TheLexer = m_lexer;
    int result = yyparse();
    if (!result) {
        auto ast = program;
        auto val = ast->codegen();
        if (val == nullptr) {
            return 0;
        }
    }
    outs() << "generating codes...\n";
    // 生成目标代码
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto TargetTriple = sys::getDefaultTargetTriple();
    TheModule->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        errs() << Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto TheTargetMachine =
            Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    auto Filename = "../test/output.o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::F_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }

    legacy::PassManager pass;
    pass.add(createReassociatePass());
    pass.add(createGVNPass());
    pass.add(createCFGSimplificationPass());
    auto FileType = CodeGenFileType::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }
    pass.run(*TheModule);
    dest.flush();

    outs() << "Wrote " << Filename << "\n";
    popen("objdump -S ../test/output.o > ../test/output.txt", "r");
    popen("g++ ../test/output.o -o ../test/output", "r");
    return 0;
}