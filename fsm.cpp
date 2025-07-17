#include "fsm.hpp"

enum class SystemState {
        INIT,
        IDLE,
        MOVEMENT,
        SHOOTING,
        CALCULATION,
        ERROR,
        STOPPED
};

class FSM {

        private:
        SystemState currentState;       // Current state of the FSM
        uint32_t lastHeartbeat;            // Last heartbeat time in milliseconds
        uint32_t delay;                 // Delay in milliseconds for each state transition
        int errorCount;                 // Count of errors encountered
        vector<pair<SystemState, uint32_t>> stateHistory;  // List of state and time pairs
        int moveCount;              // Count of movements performed, if 3 moves are performed, the FSM will transition to SHOOTING state.

        public: 
        
        /**
         * @brief set currentState to INIT, lastHeartbeat and errorCount to 0, don't forget to intialize the stateHistory vector.
         * @note This is a constructor, by default C++ will generate a default constructor if none is provided.
         * @note The downside of this constructor is empty, meaning it does not perform any initialization if needed.
         */
        FSM() {
            currentState = SystemState::INIT;
            lastHeartbeat = 0;
            errorCount = 0;

            stateHistory.clear();
            stateHistory.push_back({SystemState::INIT, lastHeartbeat});
        }

        /**
         * @brief set currentState to INIT, lastHeartbeat and errorCount to 0, set delay to param, don't forget to intialize the stateHistory vector.
         * @param state The initial state of the FSM.
         * @param delay The delay in milliseconds for each state transition.
         * @note this is a user-defined constructor, initializes FSM with a specified delay.
         */
        FSM(uint32_t delay) {
            currentState = SystemState::INIT;
            lastHeartbeat = 0;
            errorCount = 0;
            this->delay = delay;
            
            stateHistory.clear();
            stateHistory.push_back({SystemState::INIT, delay});
        }

        /**
         * @brief Destructor for the FSM class.
         * @note This is a destructor, by default C++ will generate a default destructor if none is provided.
         * @note The downside of this destructor is empty, it doesn't perform any malloc cleanup, don't forget to cleanup the vector.
         */
        ~FSM() {

        }

        /**
         * @brief Get the current state of the FSM.
         * @note This function returns the copy to the fsmState attribute of the FSM.
         * @return The current state of the FSM as a SystemState enum.
         */
        SystemState getCurrentState() const {
            return currentState;
        }

        /**
         * @brief Transition to a new state.
         * @param newState The new state to transition to.
         * @note This function updates the current state of the FSM to the new state.
         * It also updates the last heartbeat time to the current time in milliseconds.
         */
        void transitionToState(SystemState newState) {
            uint32_t times = millis();
            currentState = newState;
            stateHistory.push_back({newState, times});
        }

        /**
         * @brief set the delay for the FSM.
         * @param delay The delay in milliseconds to set for the FSM.
         */
        void setDelay(uint32_t delay) {
            this->delay = delay;
        }

        /**
         * @brief Get the delay for the FSM.
         * @param delay Reference to a uint32_t variable to store the delay in milliseconds.
         */
        void getDelay(uint32_t &delay) const {
            delay = this->delay;
        }

        /**
         * @brief Set the error count for the FSM.
         * @param count The number of errors to set.
         */
        void setErrorCount(int count) {
            this->errorCount = count;
        }

        /**
         * @brief Get the error count for the FSM.
         * @return The number of errors encountered by the FSM.
         */
        int getErrorCount() const {
            return errorCount;
        }

        /**
         * @brief Set the move count for the FSM.
         * @param count The number of moves to set.
         */
        void setMoveCount(int count) {
            this->moveCount = count;
        }

        /**
         * @brief Get the move count for the FSM.
         * @return The number of moves performed by the FSM.
         */
        int getMoveCount() const {
            return moveCount;
        }

        /**
         * @brief Add a state and its corresponding time to the state history.
         * @param state The state to add to the history.
         * @param time The time in milliseconds when the state was entered.
         * @note This function appends a pair of state and time to the stateHistory vector.
         */
        void addStateToHistory(SystemState state, uint32_t time) {
            stateHistory.push_back({state, time});
        }

        /**
         * @brief Get the state history of the FSM.
         * @return A vector of pairs containing the state and the time it was entered.
         * @note This function returns a copy of the stateHistory vector.
         */
        vector<pair<SystemState, uint32_t>> getStateHistory() const {
            return stateHistory;
        }

        /**
         * @brief Get the last heartbeat time of the FSM.
         * @return The last heartbeat time in milliseconds.
         * @note This function returns the lastHeartbeat attribute of the FSM.
         */
        uint32_t getLastHeartbeat() const {
            return lastHeartbeat;
        }

        /**
         * @brief Set the last heartbeat time of the FSM.
         * @param heartbeat The time in milliseconds to set as the last heartbeat.
         */
        void setLastHeartbeat(uint32_t heartbeat) {
            lastHeartbeat = heartbeat;
        }

        /**
         * @brief Start the FSM.
         * This function initializes the FSM and begins the state update loop.
         * Create a loop that checks the current state every 1000 milliseconds, 
         */
        void start() {
            currentState = getCurrentState();

            while (currentState != SystemState::STOPPED) {
                uint32_t currentTime = millis();
                currentState = getCurrentState();
                if (currentTime % 1000 == 0) {
                    addStateToHistory(currentState, currentTime);
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
        void update() {
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
            lastHeartbeat = millis();
            addStateToHistory(currentState, lastHeartbeat);
        }

        /**
         * @brief Print the current status of the FSM.
         * This function prints the current state, last heartbeat time, delay, error count
         */
        void printStatus() {
            currentState = getCurrentState();
            lastHeartbeat = getLastHeartbeat();
            errorCount = getErrorCount();
            getDelay(delay);

            string state;
            if (currentState == SystemState::INIT) {
                state = "INIT";
            } else if (currentState == SystemState::IDLE) {
                state = "IDLE";
            } else if(currentState == SystemState::MOVEMENT) {
                state = "MOVEMENT";
            } else if(currentState == SystemState::SHOOTING) {
                state = "SHOOTING";
            } else if(currentState == SystemState::CALCULATION) {
                state = "CALCULATION";
            } else if(currentState == SystemState::ERROR) {
                state = "ERROR";
            } else if(currentState == SystemState::STOPPED) {
                state = "STOPPED";
            }

            cout << "FSM Current Status:" << endl;
            cout << "1.Current State: " << state << endl;
            cout << "2.Last Heart Beat: " << lastHeartbeat << endl;
            cout << "3.Delay: " << delay << endl;
            cout << "4.Error Count: " << errorCount << endl;                                   
        }
        
        /**
         * @brief Print the state history of the FSM.
         * This function prints the state history, showing each state and the time it was entered.
         * It iterates through the stateHistory vector and prints each state and its corresponding time.
         */
        void printStateHistory() {
            stateHistory = getStateHistory();
            
            cout << "{State, Time}";
            
        }

        /**
         * @brief Perform the initialization process.
         * set delay to 1000 milliseconds,
         * set currentState to IDLE after initialization.
         * set lastHeartbeat to current time in milliseconds.
         * @note This function is called when the FSM is in the INIT state.
         * print "Initializing system..." then invoke printStatus()
         */
        void performInit() {
            currentState = getCurrentState();

            if (currentState == SystemState::INIT){
                cout << "Initializing system...";
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
        void performProcess() {
            currentState = getCurrentState();
            string process;
            vector<string> type = {"1.IDLE", "2.MOVEMENT", "3.SHOOTING", "4.CALCULATION"};

            if (currentState == SystemState::IDLE) {
                for(string x : type) {
                    cout << x << endl;
                }

                cout  << "Choose process: ";
                cin >> process;
                if (process == "IDLE") {
                    currentState = SystemState::IDLE;
                } else if (process == "MOVEMENT") {
                    currentState = SystemState:: MOVEMENT;
                } else if (process == "SHOOTING") {
                    currentState = SystemState::SHOOTING;
                } else if (process == "CALCULATION") {
                    currentState = SystemState::CALCULATION;
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
        void performMovement() {
            currentState = getCurrentState();
            moveCount = getMoveCount();

            if (currentState == SystemState::MOVEMENT) {
                cout << "Moving...";
                moveCount ++;
                if (moveCount >= 3) {
                    currentState = SystemState::SHOOTING;
                } else {
                    SystemState::IDLE;
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
        void performShooting() {
            currentState = getCurrentState();
            moveCount = getMoveCount();
            lastHeartbeat = getLastHeartbeat();

            if (currentState == SystemState::SHOOTING) {
                cout << "Shooting...";
                moveCount = 0;
                currentState = SystemState::IDLE;
                lastHeartbeat = millis();
            }
        }

        /**
         * @brief Perform the calculation process.
         * This function is called when the FSM is in the CALCULATION state.
         * Print "Performing calculation..."
         * If moveCount is 0, transition to the ERROR state.
         * if moveCount is greater than 0, transition to the IDLE state.
         */
        void performCalculation() {
            moveCount = getMoveCount();
            currentState = getCurrentState();

            if (currentState == SystemState::CALCULATION) {
                cout << "Performing calculation...";
                if (moveCount == 0) {
                    currentState = SystemState::ERROR;
                } else if (moveCount > 0) {
                    currentState = SystemState::IDLE;
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
        void performErrorHandling() {
            currentState = getCurrentState();
            errorCount = getErrorCount();

            if (currentState == SystemState::ERROR) {
                cout << "Error occurred, performing error handling...";
                errorCount ++;
                if (errorCount >= 3) {
                    shutdown();
                } else {
                    currentState = SystemState::IDLE;
                }
            }
        }

        /**
         * @brief shutdown the FSM if the current state is STOPPED.
         * This function is called when the FSM is in the STOPPED state.
         * Print "System stopped, shutting down..."
         * Clear the stateHistory vector.
         */
        void shutdown() {
            currentState = getCurrentState();

            if (currentState == SystemState::STOPPED) {
                cout << "System stopped, shutting down...";
                stateHistory.clear();
            }
        }
};