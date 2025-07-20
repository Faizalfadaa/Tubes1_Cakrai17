#include "fsm.hpp"

uint32_t millis() {
    return (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC);
}

FSM::FSM() {
    this->currentState = SystemState::INIT;
    lastHeartbeat = 0;
    errorCount = 0;
    moveCount = 0;

    stateHistory.clear();
    stateHistory.push_back({SystemState::INIT, lastHeartbeat});
}

FSM::FSM(uint32_t delay) {
    this->currentState = SystemState::INIT;
    lastHeartbeat = 0;
    errorCount = 0;
    moveCount = 0;
    this->delay = delay;
    
    stateHistory.clear();
    stateHistory.push_back({SystemState::INIT, lastHeartbeat});
}

FSM::~FSM() {
    stateHistory.clear();
}

SystemState FSM::getCurrentState() const {
    return currentState;
}

void FSM::transitionToState(SystemState newState) {
    uint32_t times = millis();
    this->currentState = newState;
    stateHistory.push_back({newState, times});
}

void FSM::setDelay(uint32_t delay) {
    this->delay = delay;
}

void FSM::getDelay(uint32_t &delay) const {
    delay = this->delay;
}

void FSM::setErrorCount(int count) {
    this->errorCount = count;
}

int FSM::getErrorCount() const {
    return errorCount;
}

void FSM::setMoveCount(int count) {
    this->moveCount = count;
}

int FSM::getMoveCount() const {
    return moveCount;
}

void FSM::addStateToHistory(SystemState state, uint32_t time) {
    stateHistory.push_back({state, time});
}

vector<pair<SystemState, uint32_t>> FSM::getStateHistory() const {
    return stateHistory;
}

uint32_t FSM::getLastHeartbeat() const {
    return lastHeartbeat;
}

void FSM::setLastHeartbeat(uint32_t heartbeat) {
    lastHeartbeat = heartbeat;
}

void FSM::start() {
    currentState = getCurrentState();

    while (currentState != SystemState::STOPPED) {
        uint32_t currentTime = millis();
        currentState = getCurrentState();

        if (currentTime - lastHeartbeat >= 1000) {
            update();
            setLastHeartbeat(currentTime);
        }
    }
}

void FSM::update() {
    currentState = getCurrentState();

    if (currentState == SystemState::INIT) {
        performInit();
    } else if (currentState == SystemState::IDLE) {
        performProcess();
    } else if(currentState == SystemState::MOVEMENT) {
        performMovement();
    } else if(currentState == SystemState::SHOOTING) {
        performShooting();
    } else if(currentState == SystemState::CALCULATION) {
        performCalculation();
    } else if(currentState == SystemState::ERROR) {
        performErrorHandling();
    } else if(currentState == SystemState::STOPPED) {
        shutdown();
    }
    
    currentState = getCurrentState();
    setLastHeartbeat(millis());
    addStateToHistory(currentState, lastHeartbeat);
}

string stateToString(SystemState currentState) {
    if (currentState == SystemState::INIT) {
        return "INIT";
    } else if (currentState == SystemState::IDLE) {
        return "IDLE";
    } else if(currentState == SystemState::MOVEMENT) {
        return "MOVEMENT";
    } else if(currentState == SystemState::SHOOTING) {
        return "SHOOTING";
    } else if(currentState == SystemState::CALCULATION) {
        return "CALCULATION";
    } else if(currentState == SystemState::ERROR) {
        return "ERROR";
    } else if(currentState == SystemState::STOPPED) {
        return "STOPPED";
    }
    return "UNKNOWN";
}

void FSM::printStatus() { 
    currentState = getCurrentState();
    lastHeartbeat = getLastHeartbeat();
    errorCount = getErrorCount();
    getDelay(delay);

    string state = stateToString(currentState);
    cout << endl;  
    cout << "--- FSM Current Status ---" << endl;
    cout << "1. Current State: " << state << endl;
    cout << "2. Last Heart Beat: " << lastHeartbeat << endl;
    cout << "3. Delay: " << delay << endl;
    cout << "4. Error Count: " << errorCount << endl;                                
}

void FSM::printStateHistory() {
    cout << endl;    
    cout << "--- State History {State, Time} ---" << endl;
    for(int i = 0; i < stateHistory.size(); i++) {
        const auto& entry = stateHistory[i];
        SystemState currentState = entry.first;
        uint32_t currentTime = entry.second;
        cout << i + 1<< ". {" << stateToString(currentState) << "," << currentTime << "}" << endl;
    }              
}

void FSM::performInit() {
    currentState = getCurrentState();
    if (currentState == SystemState::INIT){
        cout << endl;  
        cout << "Initializing system..." << endl;
        setDelay(1000);
        transitionToState(SystemState::IDLE);

        uint32_t currentTime = millis();
        setLastHeartbeat(currentTime);

        printStatus();
    }
}

void FSM::performProcess() {
    currentState = getCurrentState();
    string process;
    vector<string> type = {"1. IDLE", "2. MOVEMENT", "3. SHOOTING", "4. CALCULATION"};

    cout << endl;  
    cout << "--- Perform Process ---" << endl;
    if (currentState == SystemState::IDLE) {
        for(string x : type) {
            cout << x << endl;
        }

        cout  << ">> Choose process: ";
        cin >> process;
        if (process == "IDLE") {
            this->currentState = SystemState::IDLE;
        } else if (process == "MOVEMENT") {
            this->currentState = SystemState:: MOVEMENT;
        } else if (process == "SHOOTING") {
            this->currentState = SystemState::SHOOTING;
        } else if (process == "CALCULATION") {
            this->currentState = SystemState::CALCULATION;
        } else if (process == "HISTORY") {
            printStateHistory();
        } else if (process == "STATUS") {
            printStatus();
        } else {
            cout << endl;
            cout << "Invalid Process" << endl;
        }
    }
}

void FSM::performMovement() {
    currentState = getCurrentState();
    moveCount = getMoveCount();

    if (currentState == SystemState::MOVEMENT) {
        cout << endl;  
        cout << "Moving..." << endl;
        this->moveCount ++;
        if (moveCount >= 3) {
            this->currentState = SystemState::SHOOTING;
        } else {
            this->currentState = SystemState::IDLE;
        }
    }
}

void FSM::performShooting() {
    currentState = getCurrentState();
    moveCount = getMoveCount();
    lastHeartbeat = getLastHeartbeat();

    if (currentState == SystemState::SHOOTING) {
        cout << endl;  
        cout << "Shooting..." << endl;
        this->moveCount = 0;
        this->currentState = SystemState::IDLE;
        setLastHeartbeat(millis());
    }
}

void FSM::performCalculation() {
    moveCount = getMoveCount();
    currentState = getCurrentState();

    if (currentState == SystemState::CALCULATION) {
        cout << endl;  
        cout << "Performing calculation..." << endl;
        if (moveCount == 0) {
            this->currentState = SystemState::ERROR;
        } else if (moveCount > 0) {
            this->currentState = SystemState::IDLE;
        }
    }
}

void FSM::performErrorHandling() {
    currentState = getCurrentState();
    errorCount = getErrorCount();

    if (currentState == SystemState::ERROR) {
        cout << "Error occurred, performing error handling..." << endl;
        this->errorCount ++;
        if (errorCount >= 3) {
            this->currentState = SystemState::STOPPED;
            shutdown();
        } else {
            this->currentState = SystemState::IDLE;
        }
    }
}

void FSM::shutdown() {
    currentState = getCurrentState();

    if (currentState == SystemState::STOPPED) {
        cout << endl;  
        cout << "System stopped, shutting down..." << endl << endl;
        stateHistory.clear();
    }
}