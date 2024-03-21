#ifndef SAFESTATE_H
#define SAFESTATE_H

class SafeState {
  public:
    SafeState();
    void lock();
    bool unlock(String code);
    bool locked();
    bool hasCode();
    void setCode(String newCode);
    String getCode();

  private:
    void setLock(bool locked);
    bool _locked;
};

#endif