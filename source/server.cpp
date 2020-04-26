#include "server.h"

int main(int argc, char *argv[]) {

    int games = 1000;
    int seed = time(NULL);
    int deal = 3;
    std::vector<std::string> hFiles;
    bool verbose = false;
    bool checkSolv = false;
    Verifier verifier = Verifier("klondike.json", "solvOut.txt");

    int opt;
    while((opt = getopt(argc, argv, "d:g:s:h:v")) != -1) {
        switch(opt) {
            case 'd':
                deal = std::stoi(optarg);
                break;
            case 'g':
                games = std::stoi(optarg);
                break;
            case 's':
                seed = std::stoi(optarg);
                break;
            case 'h':
                hFiles.push_back(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'c':
                checkSolv = true;
                break;
            case '?':
                std::cout << "Unknown option: " << optopt << std::endl;
                break;
        }
    }

    std::cout << "Running solvers with " << deal << " deal klondike on " <<games << " games with seed " << seed;

    if (checkSolv) {
        std::cout << " with solvitaire verification";
    }
    std::cout << std::endl;

    std::vector<Solver> solvers = setSolvers(hFiles);

    SolverCompare comp(solvers, verbose);

    auto start = std::chrono::high_resolution_clock::now();
    comp = runGames(comp, deal, seed, games, checkSolv, verifier);
    auto end = std::chrono::high_resolution_clock::now();

    std::ofstream out("./output/output.csv");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    std::cout << comp.toString();
    std::cout.rdbuf(coutbuf);

    std::cout << comp.toString();
    std::cout << "(" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms total time)"<< '\n';
    return 0;
}

std::vector<Solver> setSolvers(std::vector<std::string> hFiles) {

    std::vector<Solver> solvers;
    std::vector<Heuristic> heuristics;

    if (!hFiles.empty()) {

        std::cout << "Heuristics from " << hFiles.size() << " files" << std::endl;
        for (auto &filename: hFiles) {

            std::cout << "Processing " << filename << std::endl;

            std::ifstream ifs(filename);
            rapidjson::IStreamWrapper isw(ifs);
            rapidjson::Document document;
            document.ParseStream(isw);

            for (auto &obj: document["solvers"].GetArray()) {

                std::string name = obj["name"].GetString();

                heuristics = std::vector<Heuristic>();
                rapidjson::Value::ConstMemberIterator itr = obj.FindMember("heuristics");
                if (itr != obj.MemberEnd()) {
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::FOUNDATION, "FOUNDATION");
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::REVEAL_HIDDEN, "REVEAL_HIDDEN");
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::PLAN_REVEAL_HIDDEN, "PLAN_REVEAL_HIDDEN");
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::EMPTY_SPACE_NO_KING, "EMPTY_SPACE_NO_KING");
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::STOCK_SAFE, "STOCK_SAFE");
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::TABLEAU, "TABLEAU");
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::STOCK_DISTANCE, "STOCK_DISTANCE");
                    findHeuristicValue(&heuristics, &itr->value, HeuristicType::SMOOTH, "SMOOTH");
                }

                std::function<int(Solver*, Klondike, std::vector<Move>)> searchFcn = runNoSearch;
                std::function<int(Solver*, int)> searchLimitFcn = getDepth;
                int searchLimitValue = 0;

                itr = obj.FindMember("search");
                if (itr != obj.MemberEnd()) {
                    switch (itr->value["search_fcn"].GetInt()) {
                        case 0:
                            searchFcn = runSearchIDDFSCheckStock;
                            break;
                        case 1:
                            searchFcn = runSearchIDDFSNoCheckStock;
                            break;
                        case 2:
                            searchFcn = runSearchDFS;
                            break;
                    }

                    switch (itr->value["limit_fcn"].GetInt()) {
                        case 0:
                            searchLimitFcn = getDepth;
                            break;
                        case 1:
                            searchLimitFcn = getNodes;
                            break;
                    }
                    searchLimitValue = itr->value["limit_value"].GetInt();
                }

                solvers.push_back(Solver(name, heuristics, searchFcn, searchLimitFcn, searchLimitValue));
            }
        }
    } else {
        solvers.push_back(Solver("Random Solver", std::vector<Heuristic>(), runNoSearch, getDepth, 0));
    }

    return solvers;
}

void findHeuristicValue(std::vector<Heuristic>* heuristics, const rapidjson::Value* obj, HeuristicType type, std::string name) {
    rapidjson::Value::ConstMemberIterator itr = obj->FindMember(name.c_str());
    if (itr != obj->MemberEnd()) {
        (*heuristics).push_back(Heuristic(type, itr->value.GetInt()));
    }
}

SolverCompare runGames(SolverCompare comp, int deal, int seed, int games, bool verify, Verifier verifier) {

    for (int i = 0; i < games; i++) {
        float percent = static_cast<float>(i*100)/games;
        std::cout << percent << "%   \r" << std::flush;

        seed++;
        Klondike game = Klondike(seed, deal);
        bool anyWins = comp.runSolvers(game, seed);

        if (anyWins && verify == true) {
            verifier.checkGame(game);
        }
    }
    std::cout << "            \r" << std::flush;
    return comp;
}
