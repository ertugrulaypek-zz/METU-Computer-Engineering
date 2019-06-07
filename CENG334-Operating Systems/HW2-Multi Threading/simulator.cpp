#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <random>
#include <time.h>
#include "writeOutput.h"

using namespace std;

enum PType{
    SMELTER,
    FOUNDRY
};

unsigned int number_of_mines, number_of_smelters, number_of_transporters, number_of_foundries;
unsigned int producer_number;
unsigned int last_loaded_miner_id = 0;
unsigned int stopped_miner_count = 0;
bool is_transporters_active = true;


sem_t can_transport_ore;
sem_t last_load_miner_mutex;
sem_t transporter_can_put_iron_ore;
sem_t transporter_can_put_copper_ore;
sem_t transporter_can_put_coal_ore;
sem_t producer_queue_mutex;
sem_t mutex_number;
sem_t miner_stop_mutex;

std::vector<MinerInfo> miner_info_vec;
std::vector<sem_t> miner_empty_space_count;
std::vector<sem_t> miner_full_space_count;
std::vector<sem_t> miner_mutex;
std::vector<sem_t> miner_load_mutex;
std::vector<unsigned int> miner_max_can_extracted;
std::vector<unsigned int> miner_interval;
std::vector<unsigned int> miner_reserved_ore_count;
std::vector<unsigned int> miner_total_extracted;

std::deque<unsigned int> foundry_coal_queue;
std::deque<unsigned int> foundry_coal_prioritized_queue;
std::deque<unsigned int> foundry_iron_queue;
std::deque<unsigned int> foundry_iron_prioritized_queue;
std::deque<unsigned int> smelter_iron_queue;
std::deque<unsigned int> smelter_iron_prioritized_queue;
std::deque<unsigned int> smelter_copper_queue;
std::deque<unsigned int> smelter_copper_prioritized_queue;
std::vector<unsigned int> smelter_reserved_ore_count;
std::vector<unsigned int> smelter_second_ore;
std::vector<unsigned int> foundry_drop_iron_count;
std::vector<unsigned int> foundry_drop_coal_count;
std::vector<unsigned int> foundry_reserved_iron_count;
std::vector<unsigned int> foundry_reserved_coal_count;
std::vector<sem_t> smelter_second_ore_mutex;
std::vector<sem_t> smelter_mutex;
std::vector<sem_t> smelter_empty_space_count;
std::vector<sem_t> smelter_full_space_count;
std::vector<sem_t> smelter_ore_mutex;
std::vector<sem_t> foundry_ore_mutex;
std::vector<sem_t> foundry_mutex;
std::vector<sem_t> foundry_drop_mutex;
std::vector<sem_t> foundry_unload_mutex;
std::vector<sem_t> smelter_unload_mutex;

std::vector<TransporterInfo> transporter_info_vec;
std::vector<unsigned int> transporter_interval;

std::vector<SmelterInfo> smelter_info_vec;
std::vector<unsigned int> smelter_interval;

std::vector<FoundryInfo> foundry_info_vec;
std::vector<unsigned int> foundry_interval;

void MinerProduced(unsigned int m_id){
    sem_post(&miner_full_space_count[m_id - 1]);
    sem_post(&can_transport_ore);
}


void WaitCanProduce(unsigned int m_id){
   sem_wait(&miner_empty_space_count[m_id-1]);
}

void* Miner(void *given_id){
    unsigned int current_ore_count = 0;
   
    unsigned int m_id = (long unsigned int)given_id;

    sem_wait(&miner_mutex[m_id - 1]);
    FillMinerInfo(&miner_info_vec[m_id-1],m_id,miner_info_vec[m_id-1].oreType, miner_info_vec[m_id-1].capacity, current_ore_count);
    WriteOutput(&miner_info_vec[m_id-1], NULL, NULL, NULL, MINER_CREATED);
    sem_post(&miner_mutex[m_id - 1]);

    unsigned int time = miner_interval[m_id-1];

    int count = 0;
    while(count++ != miner_max_can_extracted[m_id - 1]){
        WaitCanProduce(m_id);

        sem_wait(&miner_mutex[m_id - 1]);
        FillMinerInfo(&miner_info_vec[m_id-1],m_id,miner_info_vec[m_id-1].oreType, miner_info_vec[m_id-1].capacity, miner_info_vec[m_id - 1].current_count);
        WriteOutput(&miner_info_vec[m_id-1], NULL, NULL, NULL, MINER_STARTED);
        sem_post(&miner_mutex[m_id - 1]);

        usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));

        sem_wait(&miner_mutex[m_id - 1]);
        miner_total_extracted[m_id - 1]++;
        miner_info_vec[m_id - 1].current_count++;
        miner_reserved_ore_count[m_id - 1]++;
        //miner_extracted[m_id-1]++;
        sem_post(&miner_mutex[m_id - 1]);

        MinerProduced(m_id);

        
        sem_wait(&miner_mutex[m_id - 1]);
        FillMinerInfo(&miner_info_vec[m_id-1],m_id,miner_info_vec[m_id-1].oreType, miner_info_vec[m_id-1].capacity, miner_info_vec[m_id - 1].current_count);
        WriteOutput(&miner_info_vec[m_id-1], NULL, NULL, NULL, MINER_FINISHED);
        sem_post(&miner_mutex[m_id - 1]);

        usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));
    }


    //MinerStopped(t_id);
    sem_wait(&miner_mutex[m_id - 1]);
    FillMinerInfo(&miner_info_vec[m_id-1],m_id,miner_info_vec[m_id-1].oreType, miner_info_vec[m_id-1].capacity, miner_info_vec[m_id - 1].current_count);    
    WriteOutput(&miner_info_vec[m_id-1], NULL, NULL, NULL, MINER_STOPPED);
    //miner_is_active[t_id-1]=false;
    sem_post(&miner_mutex[m_id - 1]);
    pthread_exit(NULL);
}

unsigned int searchForMinerWithOre(){

   unsigned int last_loaded_id = last_loaded_miner_id;
   
   for(int i = last_loaded_id; i<number_of_mines;i++){   
      sem_wait(&miner_mutex[i]);
      if(miner_reserved_ore_count[i] > 0){
          sem_wait(&last_load_miner_mutex);
          last_loaded_miner_id = i + 1;
          if(last_loaded_miner_id == number_of_mines){
              last_loaded_miner_id = 0;
          }

          sem_post(&last_load_miner_mutex);
        
          sem_wait(&miner_stop_mutex);
          miner_reserved_ore_count[i]--;
          if(miner_reserved_ore_count[i] == 0 && miner_max_can_extracted[i] == miner_total_extracted[i]){
              stopped_miner_count++;
              if(stopped_miner_count == number_of_mines){
                  is_transporters_active = false;
                  sem_post(&can_transport_ore);
              }
          }
          sem_post(&miner_stop_mutex);
          sem_post(&miner_mutex[i]);
          return i+1;
      }
      sem_post(&miner_mutex[i]);
   }

   for(int i=0; i<last_loaded_id;i++){
      sem_wait(&miner_mutex[i]);
      if(miner_reserved_ore_count[i] > 0){
          sem_wait(&last_load_miner_mutex);          
          last_loaded_miner_id = i + 1;
          if(last_loaded_miner_id == number_of_mines){
              last_loaded_miner_id = 0;
          }
          sem_post(&last_load_miner_mutex);          
          miner_reserved_ore_count[i]--;
          sem_wait(&miner_stop_mutex);
          if(miner_reserved_ore_count[i] == 0 && miner_max_can_extracted[i] == miner_total_extracted[i]){
              stopped_miner_count++;
              if(stopped_miner_count == number_of_mines){
                  is_transporters_active = false;
                  sem_post(&can_transport_ore);
              }
          }
          sem_post(&miner_stop_mutex);
          sem_post(&miner_mutex[i]);
          return i+1;
      }
      sem_post(&miner_mutex[i]);
   }
   
   return 0;
}

unsigned int WaitNextLoad(unsigned int t_id){
   sem_wait(&can_transport_ore);
   sem_wait(&miner_stop_mutex);
   if(number_of_mines == stopped_miner_count){
       sem_post(&miner_stop_mutex);
       sem_post(&can_transport_ore);
       return 0;
   }
   sem_post(&miner_stop_mutex);
   while(1){
      int assigned_miner_id = searchForMinerWithOre();
      if(assigned_miner_id==0) return 0;
      else{
        return assigned_miner_id;
      }
   }
   
}

void Transporter_Miner(unsigned int t_id, unsigned int m_id, OreType* carried_ore){
   MinerInfo miner_info = miner_info_vec[m_id-1];
   FillMinerInfo(&miner_info,m_id,OreType(0),0,0);                                            /// Q: whether to use miner_info or miner_info_vec
   FillTransporterInfo(&transporter_info_vec[t_id-1], t_id, carried_ore);
   WriteOutput(&miner_info, &transporter_info_vec[t_id-1], NULL, NULL, TRANSPORTER_TRAVEL);
   unsigned int time = transporter_interval[t_id-1];
   usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));

   sem_wait(&miner_load_mutex[m_id-1]);
   miner_info_vec[m_id-1].current_count--;
   *carried_ore = miner_info_vec[m_id-1].oreType;
   sem_wait(&miner_mutex[m_id - 1]);
   FillMinerInfo(&miner_info, m_id, *carried_ore, miner_info_vec[m_id-1].capacity, miner_info_vec[m_id-1].current_count);
   FillTransporterInfo(&transporter_info_vec[t_id-1],t_id,carried_ore);
   WriteOutput(&miner_info, &transporter_info_vec[t_id-1], NULL, NULL,TRANSPORTER_TAKE_ORE);
   sem_post(&miner_mutex[m_id - 1]);
   usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));
   sem_post(&miner_load_mutex[m_id-1]);
   // Loaded(Miner); is below
   sem_post(&miner_empty_space_count[m_id - 1]);
}

void Transporter_Smelter(unsigned int t_id, unsigned int p_id){
    SmelterInfo smelter_info = smelter_info_vec[p_id-1];
    FillSmelterInfo(&smelter_info, p_id, OreType(0),0,0,0);
    FillTransporterInfo(&transporter_info_vec[t_id-1],t_id,transporter_info_vec[t_id - 1].carry);
    WriteOutput(NULL, &transporter_info_vec[t_id-1], &smelter_info, NULL, TRANSPORTER_TRAVEL);
    unsigned int time = transporter_interval[t_id-1];
    usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));
    sem_wait(&smelter_unload_mutex[p_id-1]);
    sem_wait(&smelter_mutex[p_id - 1]);
    smelter_info_vec[p_id-1].waiting_ore_count++;
    FillSmelterInfo(&smelter_info,p_id,*transporter_info_vec[t_id - 1].carry,smelter_info_vec[p_id-1].loading_capacity, smelter_info_vec[p_id-1].waiting_ore_count, smelter_info_vec[p_id-1].total_produce);
    FillTransporterInfo(&transporter_info_vec[t_id-1],t_id,transporter_info_vec[t_id - 1].carry);
    WriteOutput(NULL, &transporter_info_vec[t_id-1], &smelter_info, NULL,TRANSPORTER_DROP_ORE);
    sem_post(&smelter_mutex[p_id-1]);
    usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));

    sem_wait(&smelter_mutex[p_id-1]);
    sem_wait(&producer_queue_mutex);
    sem_wait(&smelter_second_ore_mutex[p_id-1]);
    if(smelter_second_ore[p_id-1] == 1){
        smelter_second_ore[p_id-1] =0;
        sem_post(&smelter_ore_mutex[p_id-1]);
    }
    else{
        smelter_second_ore[p_id-1] = 1;
    }
    sem_post(&smelter_second_ore_mutex[p_id-1]);
    if(*transporter_info_vec[t_id - 1].carry == IRON){
        if(smelter_info_vec[p_id-1].waiting_ore_count == 1){
            smelter_iron_prioritized_queue.push_back(p_id);
        }
        else if(!(smelter_info_vec[p_id-1].waiting_ore_count == smelter_info_vec[p_id - 1].loading_capacity)){
            smelter_iron_queue.push_back(p_id);
        }
    }
    else if(*transporter_info_vec[t_id - 1].carry == COPPER){
        if(smelter_info_vec[p_id-1].waiting_ore_count == 1){
            smelter_copper_prioritized_queue.push_back(p_id);
        }
        else if(!(smelter_info_vec[p_id-1].waiting_ore_count == smelter_info_vec[p_id - 1].loading_capacity)){
            smelter_copper_queue.push_back(p_id);
        }
    }
    sem_post(&producer_queue_mutex);
    sem_post(&smelter_mutex[p_id-1]);
    sem_post(&smelter_unload_mutex[p_id-1]);
    // Unloaded(Smelter);
    return;
}

void Transporter_Foundry(unsigned int t_id, unsigned int p_id){
   FoundryInfo foundry_info = foundry_info_vec[p_id-1];
   FillFoundryInfo(&foundry_info,p_id,0,0,0,0);
   FillTransporterInfo(&transporter_info_vec[t_id-1],t_id,transporter_info_vec[t_id-1].carry);
   WriteOutput(NULL, &transporter_info_vec[t_id-1], NULL, &foundry_info, TRANSPORTER_TRAVEL);
   unsigned int time = transporter_interval[t_id-1];
   usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));

    sem_wait(&foundry_unload_mutex[p_id-1]);
    sem_wait(&foundry_mutex[p_id-1]);
   if(*transporter_info_vec[t_id-1].carry == OreType(IRON)){
      foundry_info_vec[p_id-1].waiting_iron++;
   }
   else{
      foundry_info_vec[p_id-1].waiting_coal++;
   }
   FillFoundryInfo(&foundry_info,p_id, foundry_info_vec[p_id-1].loading_capacity,foundry_info_vec[p_id-1].waiting_iron,foundry_info_vec[p_id-1].waiting_coal,foundry_info_vec[p_id-1].total_produce);
   FillTransporterInfo(&transporter_info_vec[t_id-1],t_id,transporter_info_vec[t_id-1].carry);
   WriteOutput(NULL, &transporter_info_vec[t_id-1], NULL, &foundry_info, TRANSPORTER_DROP_ORE);
   sem_post(&foundry_mutex[p_id-1]);
   
   usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));
   sem_wait(&foundry_mutex[p_id-1]);
    sem_wait(&producer_queue_mutex);
   if(*transporter_info_vec[t_id-1].carry == OreType(IRON)){
      sem_wait(&foundry_drop_mutex[p_id-1]);
      foundry_drop_iron_count[p_id-1]++;
      if(foundry_drop_coal_count[p_id-1]>0){
          foundry_drop_coal_count[p_id-1]--;
          foundry_drop_iron_count[p_id-1]--;
          sem_post(&foundry_ore_mutex[p_id-1]);
      }
      sem_post(&foundry_drop_mutex[p_id-1]);
      if(foundry_info_vec[p_id-1].waiting_coal==0 && foundry_info_vec[p_id-1].waiting_iron==1){
        
            for(int i = 0; i < foundry_coal_queue.size(); i++){
            if(foundry_coal_queue[i] == p_id){
                foundry_coal_queue.erase(foundry_coal_queue.begin() + i);
                break;
            }
        }
        foundry_coal_prioritized_queue.push_back(p_id);
            
      }
      else if(foundry_info_vec[p_id-1].waiting_iron==foundry_info_vec[p_id-1].loading_capacity){
          for(int i = 0; i < foundry_iron_queue.size(); i++){
            if(foundry_iron_queue[i] == p_id){
                foundry_iron_queue.erase(foundry_iron_queue.begin() + i);
                break;
            }
        }
      }
   }
   else{
        sem_wait(&foundry_drop_mutex[p_id-1]);
        foundry_drop_coal_count[p_id-1]++;
        if(foundry_drop_iron_count[p_id-1]>0){
            foundry_drop_coal_count[p_id-1]--;
            foundry_drop_iron_count[p_id-1]--;
            sem_post(&foundry_ore_mutex[p_id-1]);
        }
        sem_post(&foundry_drop_mutex[p_id-1]);
      
      if(foundry_info_vec[p_id-1].waiting_coal==1 && foundry_info_vec[p_id-1].waiting_iron==0){
      for(int i = 0; i < foundry_iron_queue.size(); i++){
            if(foundry_iron_queue[i] == p_id){
                foundry_iron_queue.erase(foundry_iron_queue.begin() + i);
                break;
            }
        }
        foundry_coal_prioritized_queue.push_back(p_id);
      }
      else if(foundry_info_vec[p_id-1].waiting_coal==foundry_info_vec[p_id-1].loading_capacity){
          for(int i = 0; i < foundry_coal_queue.size(); i++){
            if(foundry_coal_queue[i] == p_id){
                foundry_coal_queue.erase(foundry_coal_queue.begin() + i);
                break;
            }
        }
      }
   }
   sem_post(&foundry_mutex[p_id-1]);
   sem_post(&producer_queue_mutex);
   sem_post(&foundry_unload_mutex[p_id-1]);
   
   // Unloaded(Foundry);
   return;
}


void WaitProducer(PType &producer_type, unsigned int &p_id, OreType transporter_ore_type){
    if(transporter_ore_type == IRON){
        sem_wait(&transporter_can_put_iron_ore);
    }
    else if(transporter_ore_type == COPPER){
        sem_wait(&transporter_can_put_copper_ore);
    }
    else{
        sem_wait(&transporter_can_put_coal_ore);
    }
    
    //std::cout << " A R R I V E D \n";
    if(transporter_ore_type == IRON){
        if(!smelter_iron_prioritized_queue.empty()){
            producer_type = SMELTER;
            p_id = smelter_iron_prioritized_queue.front();
            smelter_reserved_ore_count[p_id - 1]++;

            if(smelter_reserved_ore_count[p_id - 1] == smelter_info_vec[p_id - 1].loading_capacity){
                smelter_iron_prioritized_queue.pop_front();
            }
            return;
        }
        else if(!foundry_iron_prioritized_queue.empty()){
            producer_type = FOUNDRY;
            p_id = foundry_iron_prioritized_queue.front();
            foundry_reserved_iron_count[p_id-1]++;

            if(foundry_reserved_iron_count[p_id-1] == foundry_info_vec[p_id-1].loading_capacity){
                foundry_iron_prioritized_queue.pop_front();
            }
            return;
        }
        if(!smelter_iron_queue.empty()){
            producer_type = SMELTER;
            p_id = smelter_iron_queue.front();
            smelter_reserved_ore_count[p_id - 1]++;
            if(smelter_reserved_ore_count[p_id - 1] == smelter_info_vec[p_id - 1].loading_capacity){
                smelter_iron_queue.pop_front();
            }
            return;
        }
        else if(!foundry_iron_queue.empty()){
            producer_type = FOUNDRY;
            p_id = foundry_iron_queue.front();
            foundry_reserved_iron_count[p_id-1]++;
            if(foundry_reserved_iron_count[p_id-1] == foundry_info_vec[p_id-1].loading_capacity){
                foundry_iron_queue.pop_front();
            }
            return;
        }
    }
    else if(transporter_ore_type == COPPER){
        if(!smelter_copper_prioritized_queue.empty()){
            producer_type = SMELTER;
            p_id = smelter_copper_prioritized_queue.front();
            smelter_reserved_ore_count[p_id - 1]++;

            if(smelter_reserved_ore_count[p_id - 1] == smelter_info_vec[p_id - 1].loading_capacity){
                smelter_copper_prioritized_queue.pop_front();
            }
            return;
        }
        else{
            p_id = smelter_copper_queue.front();
            smelter_reserved_ore_count[p_id - 1]++;
            if(smelter_reserved_ore_count[p_id - 1] == smelter_info_vec[p_id - 1].loading_capacity){
                smelter_copper_queue.pop_front();
            }
        }
    }
    else{
        if(!foundry_coal_prioritized_queue.empty()){
            producer_type = FOUNDRY;
            p_id = foundry_coal_prioritized_queue.front();
            foundry_reserved_coal_count[p_id-1]++;

            if(foundry_reserved_coal_count[p_id-1] == foundry_info_vec[p_id-1].loading_capacity){
                foundry_coal_prioritized_queue.pop_front();
            }
            return;
        }
        else{
            producer_type = FOUNDRY;
            p_id = foundry_coal_queue.front();
            foundry_reserved_coal_count[p_id-1]++;
            if(foundry_reserved_coal_count[p_id-1] == foundry_info_vec[p_id-1].loading_capacity){
                foundry_coal_queue.pop_front();
            }
            return;
        }
    }
}

void* Transporter(void *given_id){
    unsigned int t_id = (long unsigned int)given_id;
    OreType* carried_ore = NULL;
    FillTransporterInfo(&transporter_info_vec[t_id-1],t_id,carried_ore);
    WriteOutput(NULL, &transporter_info_vec[t_id-1], NULL, NULL,TRANSPORTER_CREATED);

    while(is_transporters_active){
        carried_ore=NULL;
        
        unsigned int m_id = WaitNextLoad(t_id); // gives id of miner
        if(m_id == 0){
            break;
        }
        Transporter_Miner(t_id, m_id, &miner_info_vec[m_id-1].oreType);

        PType producer_type;
        unsigned int p_id;
        WaitProducer(producer_type, p_id, *transporter_info_vec[t_id - 1].carry);

        if(producer_type == SMELTER){
            Transporter_Smelter(t_id, p_id);
        }
        else{
            Transporter_Foundry(t_id, p_id);
        }
        /*
        Producer = WaitProducer();
        if(Producer==OreType(SMELTER)){
            TransporterSmelter();
        }
        else{
            TransporterFoundry();
        }
        */
        
    }

    FillTransporterInfo(&transporter_info_vec[t_id-1],t_id, carried_ore);
    WriteOutput(NULL, &transporter_info_vec[t_id-1], NULL, NULL,TRANSPORTER_STOPPED);
    
    pthread_exit(NULL);
}

int WaitUntilTwoOres(unsigned int s_id){
    int res;
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 5;
    res = sem_timedwait(&smelter_ore_mutex[s_id - 1], &ts);
    return res;
}

void* Smelter(void *given_id){
    unsigned int s_id = (long unsigned int)given_id;
    FillSmelterInfo(&smelter_info_vec[s_id-1],s_id,smelter_info_vec[s_id-1].oreType,smelter_info_vec[s_id-1].loading_capacity, smelter_info_vec[s_id-1].waiting_ore_count, smelter_info_vec[s_id-1].total_produce);
    WriteOutput(NULL, NULL, &smelter_info_vec[s_id-1], NULL,SMELTER_CREATED);

    sem_wait(&mutex_number);
    producer_number--;
    if(producer_number == 0){
        for(int i = 0; i < number_of_smelters; i++){
            for(int j = 0; j < smelter_info_vec[i].loading_capacity; j++){
                if(OreType(smelter_info_vec[i].oreType) == IRON){
                    sem_post(&transporter_can_put_iron_ore);
                }
                else{
                    sem_post(&transporter_can_put_copper_ore);
                }
            }
        }

        for(int i = 0; i < number_of_foundries; i++){
            for(int j = 0; j < foundry_info_vec[i].loading_capacity; j++){
                    sem_post(&transporter_can_put_iron_ore);
                    sem_post(&transporter_can_put_coal_ore);
                }
            }
    }
    sem_post(&mutex_number);

    unsigned int time = smelter_interval[s_id-1];

    while(1){
        if(WaitUntilTwoOres(s_id) == -1){
            break;
        }
        sem_wait(&smelter_mutex[s_id - 1]);

        sem_wait(&producer_queue_mutex);
        if(smelter_info_vec[s_id - 1].oreType == IRON){
            if(smelter_info_vec[s_id - 1].waiting_ore_count == smelter_info_vec[s_id - 1].loading_capacity){
                smelter_iron_queue.push_back(s_id);
            }
            else if(smelter_info_vec[s_id - 1].waiting_ore_count == 3){
                for(int i = 0; i < smelter_iron_queue.size(); i++){
                    if(smelter_iron_queue[i] == s_id){
                        smelter_iron_queue.erase(smelter_iron_queue.begin() + i);
                    }
                }
                //smelter_iron_queue.erase(smelter_iron_queue.find(s_id));
                //smelter_iron_queue.erase(s_id);
                smelter_iron_prioritized_queue.push_back(s_id);
            }
        }
        else{
            if(smelter_info_vec[s_id - 1].waiting_ore_count == smelter_info_vec[s_id - 1].loading_capacity){
                smelter_copper_queue.push_back(s_id);
            }
            else if(smelter_info_vec[s_id - 1].waiting_ore_count == 3){
                //smelter_copper_queue.erase(smelter_copper_queue.find(s_id));
                for(int i = 0; i < smelter_copper_queue.size(); i++){
                    if(smelter_copper_queue[i] == s_id){
                        smelter_copper_queue.erase(smelter_copper_queue.begin() + i);
                    }
                }
                smelter_copper_prioritized_queue.push_back(s_id);
            }
        }
        sem_post(&producer_queue_mutex);

        smelter_info_vec[s_id - 1].waiting_ore_count -= 2;
        smelter_reserved_ore_count[s_id-1] -= 2;
        FillSmelterInfo(&smelter_info_vec[s_id-1],s_id, smelter_info_vec[s_id-1].oreType,smelter_info_vec[s_id-1].loading_capacity, smelter_info_vec[s_id-1].waiting_ore_count, smelter_info_vec[s_id-1].total_produce);
        WriteOutput(NULL, NULL, &smelter_info_vec[s_id-1], NULL, SMELTER_STARTED);
        sem_post(&smelter_mutex[s_id - 1]);

        usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));
        
        sem_wait(&smelter_mutex[s_id - 1]);
        smelter_info_vec[s_id-1].total_produce++;
        if(smelter_info_vec[s_id-1].oreType == IRON){
            sem_post(&transporter_can_put_iron_ore);
            sem_post(&transporter_can_put_iron_ore);
        }
        else{
            sem_post(&transporter_can_put_copper_ore);
            sem_post(&transporter_can_put_copper_ore);
        }
        FillSmelterInfo(&smelter_info_vec[s_id-1],s_id,smelter_info_vec[s_id-1].oreType,smelter_info_vec[s_id-1].loading_capacity, smelter_info_vec[s_id-1].waiting_ore_count, smelter_info_vec[s_id-1].total_produce);
        WriteOutput(NULL, NULL, &smelter_info_vec[s_id-1], NULL, SMELTER_FINISHED);
        sem_post(&smelter_mutex[s_id - 1]);
    }

    sem_wait(&smelter_mutex[s_id - 1]);
    FillSmelterInfo(&smelter_info_vec[s_id-1],s_id,smelter_info_vec[s_id-1].oreType,smelter_info_vec[s_id-1].loading_capacity, smelter_info_vec[s_id-1].waiting_ore_count, smelter_info_vec[s_id-1].total_produce);
    WriteOutput(NULL, NULL, &smelter_info_vec[s_id - 1], NULL, SMELTER_STOPPED);
    sem_post(&smelter_mutex[s_id - 1]);
    pthread_exit(NULL);
}

int WaitForOneIronOneCoal(unsigned int f_id){
    int res;
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 5;
    res = sem_timedwait(&foundry_ore_mutex[f_id-1], &ts);
    return res;
}

void* Foundry(void *given_id){
    unsigned int f_id = (long unsigned int)given_id;
    FillFoundryInfo(&foundry_info_vec[f_id-1],f_id,foundry_info_vec[f_id-1].loading_capacity,0, 0, 0);
    WriteOutput(NULL, NULL, NULL, &foundry_info_vec[f_id-1],FOUNDRY_CREATED);

    sem_wait(&mutex_number);
    producer_number--;
    if(producer_number == 0){
        for(int i = 0; i < number_of_smelters; i++){
            for(int j = 0; j < smelter_info_vec[i].loading_capacity; j++){
                if(OreType(smelter_info_vec[i].oreType) == IRON){
                    sem_post(&transporter_can_put_iron_ore);
                }
                else{
                    sem_post(&transporter_can_put_copper_ore);
                }
            }
        }
        
        for(int i = 0; i < number_of_foundries; i++){
        for(int j = 0; j < foundry_info_vec[i].loading_capacity; j++){
            sem_post(&transporter_can_put_iron_ore);
            sem_post(&transporter_can_put_coal_ore);
        }
        }
    }
    sem_post(&mutex_number);
    
    unsigned int time = foundry_interval[f_id-1];

    while(1){
        if(WaitForOneIronOneCoal(f_id)==-1){
            break;
        }

        sem_wait(&foundry_mutex[f_id-1]);
        sem_wait(&producer_queue_mutex);
        foundry_info_vec[f_id-1].waiting_coal--;
        foundry_info_vec[f_id-1].waiting_iron--;
        foundry_reserved_coal_count[f_id-1]--;
        foundry_reserved_iron_count[f_id-1]--;

        if(foundry_info_vec[f_id-1].waiting_coal > 0 && foundry_info_vec[f_id-1].waiting_iron == 0){
            if(foundry_info_vec[f_id-1].loading_capacity == 1){
                foundry_iron_prioritized_queue.push_back(f_id);
            }
            else{
                 for(int i = 0; i < foundry_iron_queue.size(); i++){
                    if(foundry_iron_queue[i] == f_id){
                        foundry_iron_queue.erase(foundry_iron_queue.begin() + i);
                        break;
                    }
                }
                foundry_iron_prioritized_queue.push_back(f_id);
            }
        }
        else if(foundry_info_vec[f_id-1].waiting_coal == 0 && foundry_info_vec[f_id-1].waiting_iron > 0){
            if(foundry_info_vec[f_id-1].loading_capacity == 1){
                foundry_coal_prioritized_queue.push_back(f_id);
            }
            else{
                 for(int i = 0; i < foundry_coal_queue.size(); i++){
                    if(foundry_coal_queue[i] == f_id){
                        foundry_coal_queue.erase(foundry_coal_queue.begin() + i);
                        break;
                    }
                }
                foundry_coal_prioritized_queue.push_back(f_id);
            }
        }
        else if(foundry_info_vec[f_id-1].waiting_iron == foundry_info_vec[f_id-1].loading_capacity-1){
            foundry_iron_queue.push_back(f_id);
        }
        else if(foundry_info_vec[f_id-1].waiting_coal == foundry_info_vec[f_id-1].loading_capacity-1){
            foundry_coal_queue.push_back(f_id);
        }
        FillFoundryInfo(&foundry_info_vec[f_id-1],f_id,foundry_info_vec[f_id-1].loading_capacity,foundry_info_vec[f_id-1].waiting_iron, foundry_info_vec[f_id-1].waiting_iron, foundry_info_vec[f_id-1].total_produce);
        WriteOutput(NULL, NULL, NULL, &foundry_info_vec[f_id-1], FOUNDRY_STARTED);
        sem_post(&producer_queue_mutex);
        sem_post(&foundry_mutex[f_id-1]);

        usleep(time - (time*0.01) + (rand()%(int)(time*0.02)));

        foundry_info_vec[f_id-1].total_produce++;

        sem_post(&transporter_can_put_coal_ore);
        sem_post(&transporter_can_put_iron_ore);

        sem_wait(&foundry_mutex[f_id-1]);
        FillFoundryInfo(&foundry_info_vec[f_id-1],f_id,foundry_info_vec[f_id-1].loading_capacity,foundry_info_vec[f_id-1].waiting_iron, foundry_info_vec[f_id-1].waiting_iron, foundry_info_vec[f_id-1].total_produce);
        WriteOutput(NULL, NULL, NULL, &foundry_info_vec[f_id-1], FOUNDRY_FINISHED);
        sem_post(&foundry_mutex[f_id-1]);
    }

    sem_wait(&foundry_mutex[f_id-1]);
    FillFoundryInfo(&foundry_info_vec[f_id-1],f_id,foundry_info_vec[f_id-1].loading_capacity,foundry_info_vec[f_id-1].waiting_iron, foundry_info_vec[f_id-1].waiting_iron, foundry_info_vec[f_id-1].total_produce);
    WriteOutput(NULL, NULL, NULL, &foundry_info_vec[f_id-1], FOUNDRY_STOPPED);
    sem_post(&foundry_mutex[f_id-1]);
}

void InitSems(){
    sem_init(&can_transport_ore, 0, 0);
    sem_init(&last_load_miner_mutex, 0, 1);    
    sem_init(&transporter_can_put_iron_ore, 0, 0);
    sem_init(&transporter_can_put_coal_ore, 0, 0);
    sem_init(&transporter_can_put_copper_ore, 0, 0);
    sem_init(&producer_queue_mutex, 0, 1);
    sem_init(&mutex_number, 0, 1);
    sem_init(&miner_stop_mutex, 0, 1);
    for(int i = 0; i < number_of_mines; ++i){
        sem_init(&miner_empty_space_count[i], 0, miner_info_vec[i].capacity);
        sem_init(&miner_full_space_count[i], 0, 0);
        sem_init(&miner_mutex[i], 0, 1);
        sem_init(&miner_load_mutex[i], 0, 1);
    }
    for(int i = 0; i < number_of_smelters; ++i){
        sem_init(&smelter_empty_space_count[i], 0, smelter_info_vec[i].loading_capacity);
        sem_init(&smelter_full_space_count[i], 0, 0);
        sem_init(&smelter_mutex[i], 0, 1);
        sem_init(&smelter_ore_mutex[i], 0, 0);
        sem_init(&smelter_second_ore_mutex[i], 0, 1);
        sem_init(&smelter_unload_mutex[i], 0, 1);
    }
    for(int i=0; i<number_of_foundries;i++){
        sem_init(&foundry_ore_mutex[i], 0, 0);
        sem_init(&foundry_drop_mutex[i], 0, 1);
        sem_init(&foundry_mutex[i], 0,1);
        sem_init(&foundry_unload_mutex[i], 0, 1);
    }
}


int main(void){
    cin>>number_of_mines;
    pthread_t miner_threads[number_of_mines];
    miner_info_vec.resize(number_of_mines);
    miner_empty_space_count.resize(number_of_mines);
    miner_max_can_extracted.resize(number_of_mines);
    miner_mutex.resize(number_of_mines);
    miner_full_space_count.resize(number_of_mines);
    miner_reserved_ore_count.resize(number_of_mines);
    miner_total_extracted.resize(number_of_mines);
    miner_load_mutex.resize(number_of_mines);
    for(int i=0;i<number_of_mines; i++){
        unsigned int temp;
        
        cin>>temp;
        miner_interval.push_back(temp);
        cin>>temp;
        miner_info_vec[i].capacity = temp;
        cin>>temp;
        miner_info_vec[i].oreType = OreType(temp);
        miner_info_vec[i].current_count = 0;
        miner_info_vec[i].ID=(unsigned int)(i+1);

        cin>>temp;
        miner_max_can_extracted[i] = temp;
        //cout<<"max"<<miner_max_can_extracted[i]<<endl;
        //cout<< "creating thread "<<i+1<<endl;
        //miner_extracted.push_back(0);
        //miner_is_active.push_back(true);
        //miner_producing.push_back(0);
        //ore_reserved_by_transporter.push_back(0);
    }

    
    cin>>number_of_transporters;
    pthread_t transporter_threads[number_of_transporters];
    transporter_info_vec.resize(number_of_transporters);
    //t_can_transport_ore.resize(number_of_transporters);
    //t_wait_smelter_storage.resize(number_of_transporters);

    for(int i=0; i< number_of_transporters;i++){
       unsigned int temp;
       
       transporter_info_vec[i].ID= i+1;
       transporter_info_vec[i].carry = NULL;
       cin>>temp;
       transporter_interval.push_back(temp);
       //transporter_is_busy.push_back(false); 
    }

    cin>>number_of_smelters;
    
    pthread_t smelter_threads[number_of_smelters];
    smelter_info_vec.resize(number_of_smelters);
    smelter_reserved_ore_count.resize(number_of_smelters);
    smelter_mutex.resize(number_of_smelters);
    smelter_empty_space_count.resize(number_of_smelters);
    smelter_full_space_count.resize(number_of_smelters);
    smelter_ore_mutex.resize(number_of_smelters);
    smelter_second_ore.resize(number_of_smelters);
    smelter_second_ore_mutex.resize(number_of_smelters);
    smelter_unload_mutex.resize(number_of_smelters);
    //s_wait_two_ores.resize(number_of_smelters);

    for(int i=0; i<number_of_smelters;i++){
        unsigned int temp;
      
        smelter_info_vec[i].ID = i+1;
        smelter_info_vec[i].total_produce = 0;
        cin>>temp;
        smelter_interval.push_back(temp);
        cin>>temp;
        smelter_info_vec[i].loading_capacity = temp;
        cin>>temp;
        smelter_info_vec[i].oreType = OreType(temp);
        smelter_info_vec[i].waiting_ore_count = 0;
        
        if(OreType(temp) == IRON){
            smelter_iron_queue.push_back(i + 1);
        }
        else{
            smelter_copper_queue.push_back(i + 1);
        }
        
        //smelter_reserved_by_transporter.push_back(0);
    }

    cin>>number_of_foundries;
    producer_number = number_of_smelters + number_of_foundries;
    pthread_t foundry_threads[number_of_foundries];
    //sem_foundry.resize(number_of_foundries);
    foundry_info_vec.resize(number_of_foundries);
    foundry_mutex.resize(number_of_foundries);
    foundry_ore_mutex.resize(number_of_foundries);
    foundry_drop_coal_count.resize(number_of_foundries);
    foundry_drop_iron_count.resize(number_of_foundries);
    foundry_reserved_coal_count.resize(number_of_foundries);
    foundry_reserved_iron_count.resize(number_of_foundries);
    foundry_drop_mutex.resize(number_of_foundries);
    foundry_unload_mutex.resize(number_of_foundries);
    for(int i=0; i<number_of_foundries;i++){
        unsigned int temp;
        
        foundry_info_vec[i].ID= i+1;
        cin>>temp;
        foundry_interval.push_back(temp);
        cin>>temp;
        foundry_info_vec[i].loading_capacity = temp;
        foundry_info_vec[i].waiting_coal=0;
        foundry_info_vec[i].waiting_iron = 0;
        foundry_info_vec[i].total_produce=0;
        
        foundry_coal_queue.push_back(i + 1);
        foundry_iron_queue.push_back(i + 1);
    } 

    InitSems();
    
    

    
    // std::cout << "Miner info --- \n";
    // for(int i = 0; i < number_of_mines; ++i)
    // {
    //     std::cout << " Miner ID: " << miner_info_vec[i].ID << "\n";
    //     std::cout << " Miner Capacity : " << miner_info_vec[i].capacity << "\n";
    //     std::cout << " Miner OreType : " << miner_info_vec[i].oreType << "\n";
    //     std::cout << " Miner Interval: " << miner_interval[i] << "\n";
    // }

    // std::cout << "Transporter info --- \n";
    // for(int i = 0; i < number_of_transporters; ++i)
    // {
    //     std::cout << " Transporter ID: " << transporter_info_vec[i].ID << "\n";
    //     std::cout << " Transporter Interval: " << transporter_interval[i] << "\n";
    // }

    // std::cout << "Smelter info --- \n";
    // for(int i = 0; i < number_of_smelters; ++i)
    // {
    //     std::cout << " Smelter ID: " << smelter_info_vec[i].ID << "\n";
    //     std::cout << " Smelter Capacity : " << smelter_info_vec[i].loading_capacity << "\n";
    //     std::cout << " Smelter OreType : " << smelter_info_vec[i].oreType << "\n";
    //     std::cout << " Smelter Interval: " << smelter_interval[i] << "\n";
    // }

    // std::cout << "Foundry info --- \n";
    // for(int i = 0; i < number_of_foundries; ++i)
    // {
    //     std::cout << " Foundry ID: " << foundry_info_vec[i].ID << "\n";
    //     std::cout << " Foundry Capacity : " << foundry_info_vec[i].loading_capacity << "\n";
    //     std::cout << " Foundry Interval: " << foundry_interval[i] << "\n";
    // }

    for(int i = 0; i < number_of_mines; i++){
        pthread_create(&miner_threads[i], NULL, Miner, (void*)(i + 1));
    }
    for(int i = 0; i < number_of_transporters; i++){
        pthread_create(&transporter_threads[i], NULL, Transporter, (void*)(i + 1));
    }
    for(int i =0; i < number_of_smelters; i++){
        pthread_create(&smelter_threads[i], NULL, Smelter, (void*)(i + 1));
    }
    for(int i =0; i < number_of_foundries; i++){
        pthread_create(&foundry_threads[i], NULL, Foundry, (void*)(i + 1));
    }
    for(int i = 0; i < number_of_mines; i++){
        pthread_join(miner_threads[i], NULL);
    }
    for(int i = 0; i < number_of_transporters; i++){
        pthread_join(transporter_threads[i], NULL);
    }
    for(int i =0; i < number_of_smelters; i++){
        pthread_join(smelter_threads[i], NULL);
    }
    for(int i =0; i < number_of_foundries; i++){
        pthread_join(foundry_threads[i], NULL);
    }

}