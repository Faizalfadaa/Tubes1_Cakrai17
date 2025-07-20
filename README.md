# Tubes1_Cakrai17 

## Penjelasan Implementasi !!!

1. uint32_t millis()
    - Mengembalikan waktu dalam milidetik sejak program dimulai, dengan mengonversi hasil clock() menggunakan CLOCKS_PER_SEC.

2. FSM::FSM()
    - Sebagai konstruktor FSM (state FSM = INIT)

3. FSM::FSM(uint32_t delay)
    - Sebagai konstruktor FSM (state FSM = INIT) dan menginisialisasi delay 

4. FSM::~FSM()
    - Destruktor untuk FSM nya
    - Menghilangkan stateHistory FSM dengan stateHistory.clear()

5. SystemState FSM::getCurrentState() const
    - Return state sekarang dari FSM (currentState)

6. void FSM::transitionToState(SystemState newState)
    - Melakukan transisi state dengan mengubah currentState menjadi newState
    - Menambahkan dan mencatat update transisinya (this->currentState = newState) dan waktu transisinya (uint32_t times = millis()) ke stateHistory dengan state stateHistory.push_back({newState, times})

7. void FSM::setDelay(uint32_t delay)
    - Mencatat dan menyimpan nilai delay menggunakan this->delay = delay

8. void FSM::getDelay(uint32_t &delay) const
    - Referensi ke variabel bertipe uint32_t untuk menyimpan delay dalam satuan milidetik dengan delay = this->delay

9. void FSM::setErrorCount(int count)
    - Mencatat dan menyimpan jumlah error dengan this->errorCount = count

10. int FSM::getErrorCount() const
    - Return jumlah error dari FSM (errorCount)

11. void FSM::setMoveCount(int count)
    - Mencatat dan menyimpan jumlah move menggunakan this->moveCount = count

12. int FSM::getMoveCount() const
    - Return jumlah move dari FSM (moveCount)

13. void FSM::addStateToHistory(SystemState state, uint32_t time)
    - Menambahkan state FSM (state) dengan waktu state tersebut (time) kedalam stateHistory menggunakan stateHistory.push_back({state, time})

14. vector<pair<SystemState, uint32_t>> FSM::getStateHistory() const
    - Return stateHistory FSM (return stateHistory)

15. uint32_t FSM::getLastHeartbeat() const
    - Return jumlah move dari FSM (moveCount)

16. void FSM::setLastHeartbeat(uint32_t heartbeat)
    - Mencatat dan menyimpan waktu terakhir heartbeat FSM (lastheartBeat) menggunakan lastHeartbeat = heartbeat

17. void FSM::start()
    - Menginisialisasi FSM dan memulai loop pembaruan state
    - Loop akan mengecek state FSM setiap 1000 millisecond (currentTime - lastHeartbeat >= 1000) dan kemudian melakukan update pada FSM berdasarkan state tersebut (update()).
    - lastHeartBeat juga di-update sesuai waktu real-time dengan millis() (uint32_t currentTime = millis() -> setLastHeartbeat(currentTime))
    - looping berhenti jika state FSM adalah STOPPED (while (currentState != SystemState::STOPPED))

18. void FSM::update()
    - Melakukan proses pada FSM berdasarkan state FSM sekarang (currentState)
    - Kemudian state dan lastHeartBeat akan ditambahkan ke stateHistory dengan addStateToHistory(currentState, lastHeartbeat)
    - lastHeartBeat dihitung dari millis()

19. void FSM::printStatus()
    - Menampilkan currentState, lastHeartBeat, delay, dan errorCount ke layar

20. void FSM::printStateHistory()
    - Menampilkan ke layar stateHistory FSM dari awal dengan looping vector stateHistory dan menampilkan dalam format {currentState, currentTime}

21. void FSM::performInit()
    - setDelay 1000 millisecond (setDelay(1000))
    - Melakukan transisidari state INIT ke state IDLE (transitionToState(SystemState::IDLE))
    - setLastHeartBeat sesuai dengan millis()
    - Menampilkan ke layar status FSM dengan printStatus()

22. void FSM::performProcess()
    - Fungsi yang dipanggil ketika dalam state IDLE
    - Meminta input user dalam format string ("IDLE" / "MOVEMENT" / "SHOOTING" / "CALCULATION") dan mengubah state FSM sesuai dengan inputnya
    - Kalau input tidak valid maka akan keluar "Invalid Process" ke layar dan diminta memasukan input lagi
    - User juga bisa memasukan input "HISTORY" untuk melihat stateHistory FSM dan input "STATUS" untuk melihat status FSM sekarang (if needed)

23. void FSM::performMovement()
    - Fungsi yang dipanggil jika FSM berada dalam state MOVEMENT (if (currentState == SystemState::MOVEMENT))
    - Menambahkan nilai moveCount 1 (this->moveCount ++)
    - Jika moveCount lebih dari 3 state pindah menjadi SHOOTING, else: pindah ke state IDLE 
        if (moveCount >= 3) {
            this->currentState = SystemState::SHOOTING;
        } else {
            this->currentState = SystemState::IDLE;
        }

24. void FSM::performShooting()
    - Fungsi yang dipanggil jika FSM berada dalam state SHOOTING (if (currentState == SystemState::SHOOTING))
    - moveCount dibuah nilainya jadi 0 (this->moveCount = 0)
    - state diubah menjadi IDLE (this->currentState = SystemState::IDLE)
    - lastHeartBeat diubah sesuai millis() (setLastHeartbeat(millis()))

25. void FSM::performCalculation()
    - Fungsi yang dipanggil jika FSM berada dalam state     CALCULATION (if (currentState == SystemState::CALCULATION))
    - Jika moveCount = 0, state FSM menjadi ERROR. Jika tidak, state FSM menjadi IDLE
        if (moveCount == 0) {
            this->currentState = SystemState::ERROR;
        } else if (moveCount > 0) {
            this->currentState = SystemState::IDLE;
        }

26. void FSM::performErrorHandling()
    - Fungsi yang dipanggil jika FSM berada dalam state ERROR (if (currentState == SystemState::ERROR))
    - Menambahkan jumlah errorCount sebanyak +1 (this->errorCount ++)
    - Jika errorCount >= 3, maka state FSM menjadi STOPPED. Jika tidak, maka state FSM menjadi IDLE
        if (errorCount >= 3) {
            this->currentState = SystemState::STOPPED;
            shutdown();
        } else {
            this->currentState = SystemState::IDLE;
        }

27. void FSM::shutdown()
    - Fungsi dipanggil jika state FSM STOPPED (if (currentState == SystemState::STOPPED))
    - Mematikan FSM dan menghentikan program
    - Menghapus isi vector stateHistory dengan stateHistory.clear()

28. string stateToString(SystemState currentState)
    - Fungsi tambahan yang melakukan return nama state FSM dalam format string sesuai dengan state FSM nya

## How to Run the code ???
1. if pakai WSL atau Linux:
    - make
    - ./main
2. if pakai windows terminal:
    - g++ -o main main.cpp fsm.cpp
    - ./main

