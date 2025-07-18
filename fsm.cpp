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

/**
 * @brief set currentState to INIT, lastHeartbeat and errorCount to 0, set delay to param, don't forget to intialize the stateHistory vector.
 * @param state The initial state of the FSM.
 * @param delay The delay in milliseconds for each state transition.
 * @note this is a user-defined constructor, initializes FSM with a specified delay.
 */
FSM::FSM(uint32_t delay) {
    this->currentState = SystemState::INIT;
    lastHeartbeat = 0;
    errorCount = 0;
    moveCount = 0;
    this->delay = delay;
    
    stateHistory.clear();
    stateHistory.push_back({SystemState::INIT, lastHeartbeat});
}

/**
 * @brief Destructor for the FSM class.
 * @note This is a destructor, by default C++ will generate a default destructor if none is provided.
 * @note The downside of this destructor is empty, it doesn't perform any malloc cleanup, don't forget to cleanup the vector.
 */
FSM::~FSM() {
    stateHistory.clear();
}

/**
 * @brief Get the current state of the FSM.
 * @note This function returns the copy to the fsmState attribute of the FSM.
 * @return The current state of the FSM as a SystemState enum.
 */
SystemState FSM::getCurrentState() const {
    return currentState;
}

/**
 * @brief Transition to a new state.
 * @param newState The new state to transition to.
 * @note This function updates the current state of the FSM to the new state.
 * It also updates the last heartbeat time to the current time in milliseconds.
 */
void FSM::transitionToState(SystemState newState) {
    uint32_t times = millis();
    this->currentState = newState;
    stateHistory.push_back({newState, times});
}

/**
 * @brief set the delay for the FSM.
 * @param delay The delay in milliseconds to set for the FSM.
 */
void FSM::setDelay(uint32_t delay) {
    this->delay = delay;
}

/**
 * @brief Get the delay for the FSM.
 * @param delay Reference to a uint32_t variable to store the delay in milliseconds.
 */
void FSM::getDelay(uint32_t &delay) const {
    delay = this->delay;
}

/**
 * @brief Set the error count for the FSM.
 * @param count The number of errors to set.
 */
void FSM::setErrorCount(int count) {
    this->errorCount = count;
}

/**
 * @brief Get the error count for the FSM.
 * @return The number of errors encountered by the FSM.
 */
int FSM::getErrorCount() const {
    return errorCount;
}

/**
 * @brief Set the move count for the FSM.
 * @param count The number of moves to set.
 */
void FSM::setMoveCount(int count) {
    this->moveCount = count;
}

/**
 * @brief Get the move count for the FSM.
 * @return The number of moves performed by the FSM.
 */
int FSM::getMoveCount() const {
    return moveCount;
}

/**
 * @brief Add a state and its corresponding time to the state history.
 * @param state The state to add to the history.
 * @param time The time in milliseconds when the state was entered.
 * @note This function appends a pair of state and time to the stateHistory vector.
 */
void FSM::addStateToHistory(SystemState state, uint32_t time) {
    stateHistory.push_back({state, time});
}

/**
 * @brief Get the state history of the FSM.
 * @return A vector of pairs containing the state and the time it was entered.
 * @note This function returns a copy of the stateHistory vector.
 */
vector<pair<SystemState, uint32_t>> FSM::getStateHistory() const {
    return stateHistory;
}

/**
 * @brief Get the last heartbeat time of the FSM.
 * @return The last heartbeat time in milliseconds.
 * @note This function returns the lastHeartbeat attribute of the FSM.
 */
uint32_t FSM::getLastHeartbeat() const {
    return lastHeartbeat;
}

/**
 * @brief Set the last heartbeat time of the FSM.
 * @param heartbeat The time in milliseconds to set as the last heartbeat.
 */
void FSM::setLastHeartbeat(uint32_t heartbeat) {
    lastHeartbeat = heartbeat;
}

/**
 * @brief Start the FSM.
 * This function initializes the FSM and begins the state update loop.
 * Create a loop that checks the current state every 1000 milliseconds, 
 */
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

/**
 * @brief Update the FSM state based on the current state.
 * Check the attribute fsmState to determine its next process:
 * - INIT: calls performInit()
 * - IDLE: calls performProcess()
 * - MOVEMENT: calls performMovement()
 * - SHOOTING: calls performShooting()
 * - CALCULATION: calls performCalculation()
 * - ERROR: Handle error conditions.
 * - STOPPED: Stop the system operations.
 * Update the lastHeartbeat attribute to the current time in milliseconds.
 * Emplace the stateHistory vector with the current state and current time in milliseconds.
 */
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

/**
 * @brief Print the current status of the FSM.
 * This function prints the current state, last heartbeat time, delay, error count
 */
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

/**
 * @brief Print the state history of the FSM.
 * This function prints the state history, showing each state and the time it was entered.
 * It iterates through the stateHistory vector and prints each state and its corresponding time.
 */
void FSM::printStateHistory() {
    cout << endl;    
    cout << "--- State History {State, Time} ---" << endl;
    for(int i = 0; i < stateHistory.size(); i++) {
        const auto& entry = stateHistory[i];
        SystemState currentState = entry.first;
        uint32_t currentTime = entry.second;
        cout << "{" << stateToString(currentState) << "," << currentTime << "}" << endl;
    }              
}

/**
 * @brief Perform the initialization process.
 * set delay to 1000 milliseconds,
 * set currentState to IDLE after initialization.
 * set lastHeartbeat to current time in milliseconds.
 * @note This function is called when the FSM is in the INIT state.
 * print "Initializing system..." then invoke printStatus()
 */
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

/**
 * @brief Prompt the user to pick a process to be done.
 * If a command is received, set currentState to the process' state.
 * user is prompted to choose from: 
 * - displaying status and state history (IDLE)
 * - moving (MOVEMENT)
 * - shooting (SHOOTING)
 * - calculating (CALCULATION)
 * @note This function is called when the FSM is in the IDLE state.
 */
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
        }
    }
}

/**
 * @brief Perform the movement process.
 * This function is called when the FSM is in the MOVEMENT state.
 * Print "Moving..."
 * Increment moveCount by 1.
 * If moveCount (after increment) reaches 3, transition to the SHOOTING state.
 * if not, transition to the IDLE state.
 * Update the last heartbeat time to the current time in milliseconds.
 */
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

/**
 * @brief Perform the shooting process.
 * This function is called when the FSM is in the SHOOTING state.
 * Print "Shooting..."
 * Reset moveCount to 0.
 * Transition to the IDLE state.
 * Update the last heartbeat time to the current time in milliseconds.
 */
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

/**
 * @brief Perform the calculation process.
 * This function is called when the FSM is in the CALCULATION state.
 * Print "Performing calculation..."
 * If moveCount is 0, transition to the ERROR state.
 * if moveCount is greater than 0, transition to the IDLE state.
 */
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

/**
 * @brief Handle error conditions.
 * This function is called when the FSM is in the ERROR state.
 * Print "Error occurred, performing error handling..."
 * Increment errorCount by 1.
 * If errorCount exceeds 3, transition to the STOPPED state.
 * If not, transition to the IDLE state.
 */
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

/**
 * @brief shutdown the FSM if the current state is STOPPED.
 * This function is called when the FSM is in the STOPPED state.
 * Print "System stopped, shutting down..."
 * Clear the stateHistory vector.
 */
void FSM::shutdown() {
    currentState = getCurrentState();

    if (currentState == SystemState::STOPPED) {
        cout << endl;  
        cout << "System stopped, shutting down..." << endl << endl;
        stateHistory.clear();
    }
}
