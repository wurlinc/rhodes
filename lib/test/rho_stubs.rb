 module System
   def get_locale
    'en'
  end
 end
 include System

 class RhoLog
   def error(cat,msg)
     puts cat + "|" + msg
   end
   def info(cat,msg)
     puts cat + "|" + msg
   end
 end

 module SyncEngine
   @@mutex = ::Mutex.new()
   def self.lock_sync_mutex
     @@mutex.lock
   end
   def self.unlock_sync_mutex
     @@mutex.unlock
   end
	 def self.clean_objectnotify(*args)
	 end
	 def self.set_pollinterval(value)
	 end
	 def self.start_sync
	 end
	 def self.stop_sync
	 end
	 def self.update_blob_attribs(*args)
	 end
	 def self.dosearch(*args)
	 end
 end

 class RhoConf
   @@conf = {}
   def self.set_property_by_name(name,value)
     @@conf[name] = value
   end
 end

module Rho
   class RhoFSConnector
     def self.__rhoGetCurrentDir
       Dir.pwd
     end
   end
 end

