// Copyright 2009 Minor Gordon.
// This source comes from the XtreemFS project. It is licensed under the GPLv2 (see COPYING for terms and conditions).

#include "org/xtreemfs/client/osd_proxy_mux.h"
using namespace org::xtreemfs::client;


namespace org
{
  namespace xtreemfs
  {
    namespace client
    {
//      class OSDPingResponse : public YIELD::Response
//      {
//      public:
//        OSDPingResponse( const org::xtreemfs::interfaces::VivaldiCoordinates& remote_coordinates, const YIELD::Time& rtt, const std::string& target_osd_uuid )
//          : remote_coordinates( remote_coordinates ), rtt( rtt ), target_osd_uuid( target_osd_uuid )
//        { }
//  
//        const org::xtreemfs::interfaces::VivaldiCoordinates& get_remote_coordinates() const { return remote_coordinates; }
//        const YIELD::Time& get_rtt() const { return rtt; }
//        const std::string& get_target_osd_uuid() const { return target_osd_uuid; }
//
//        // YIELD::Object
//        YIELD_OBJECT_PROTOTYPES( OSDPingResponse, YIELD_OBJECT_TAG( org::xtreemfs::interfaces::OSDInterface::xtreemfs_pingResponse ) );
//
//      private:
//        org::xtreemfs::interfaces::VivaldiCoordinates remote_coordinates;
//        YIELD::Time rtt;
//        std::string target_osd_uuid;
//      };
//
//
//      class OSDPingResponseTarget : public YIELD::EventTarget
//      {
//      public:
//        OSDPingResponseTarget( YIELD::auto_Object<OSDProxyMux> osd_proxy_mux, const std::string& target_osd_uuid )
//          : osd_proxy_mux( osd_proxy_mux ), target_osd_uuid( target_osd_uuid )
//        { }
//
//        // YIELD::Object
//        YIELD_OBJECT_PROTOTYPES( OSDPingResponseTarget, 0 );
//
//        // YIELD::EventTarget
//        void send( YIELD::Event& ev )       
//        {
//          switch ( ev.get_tag() )
//          {
//            case YIELD_OBJECT_TAG( org::xtreemfs::interfaces::OSDInterface::xtreemfs_pingResponse ):
//            {
//              YIELD::Time rtt = YIELD::Time() - creation_time;
//              OSDPingResponse* response = new OSDPingResponse( static_cast<org::xtreemfs::interfaces::OSDInterface::xtreemfs_pingResponse&>( ev ).get_remote_coordinates(), rtt, target_osd_uuid );
//              YIELD::Object::decRef( ev );
//              osd_proxy_mux->send( *response );
//            }
//            break;
//
//            default: YIELD::Object::decRef( ev );
//          }
//        }
//
//      private:
//        YIELD::auto_Object<OSDProxyMux> osd_proxy_mux;
//        std::string target_osd_uuid;
//
//        YIELD::Time creation_time;
//      };
//
//
//      class OSDReadResponseTarget : public YIELD::EventTarget
//      {
//      public:
//        OSDReadResponseTarget( YIELD::auto_Object<org::xtreemfs::interfaces::OSDInterface::readRequest> read_request )
//          : original_response_target( read_request->get_response_target() ), read_request( read_request )
//        { }
//
//        // YIELD::Object
//        YIELD_OBJECT_PROTOTYPES( OSDReadResponseTarget, 1 );
//
//        // YIELD::EventTarget
//        void send( YIELD::Event& ev )
//        {
//          if ( ev.get_tag() == YIELD_OBJECT_TAG( org::xtreemfs::interfaces::OSDInterface::readResponse ) )
//            static_cast<org::xtreemfs::interfaces::OSDInterface::readResponse&>( ev ).set_selected_file_replica( read_request->get_selected_file_replica() );
//          original_response_target->send( ev );
//        }
//
//      private:
//        YIELD::auto_EventTarget original_response_target;
//        YIELD::auto_Object<org::xtreemfs::interfaces::OSDInterface::readRequest> read_request;
//      };
//
//
      class OSDTruncateResponseTarget : public YIELD::EventTarget
      {
      public:
        OSDTruncateResponseTarget( size_t expected_response_count, YIELD::auto_EventTarget final_response_target )
          : expected_response_count( expected_response_count ), final_response_target( final_response_target )
        { }

        virtual ~OSDTruncateResponseTarget()
        {
          for ( std::vector<YIELD::Event*>::iterator response_i = responses.begin(); response_i != responses.end(); response_i++ )
            YIELD::Object::decRef( **response_i );
        }

        // YIELD::Object
        YIELD_OBJECT_PROTOTYPES( OSDTruncateResponseTarget, 0 );

        // YIELD::EventTarget
        void send( YIELD::Event& ev )
        {
          responses_lock.acquire();

          responses.push_back( &ev );

          if ( responses.size() == expected_response_count )
          {
            for ( std::vector<YIELD::Event*>::iterator response_i = responses.begin(); response_i != responses.end(); response_i++ )
            {
              if ( ( *response_i )->get_tag() == YIELD_OBJECT_TAG( YIELD::ExceptionResponse ) )
              {
                respond( static_cast<YIELD::ExceptionResponse*>( *response_i )->incRef(), final_response_target );
                responses_lock.release();
                return;
              }
            }

            respond( responses, final_response_target );
          }

          responses_lock.release();
        }

      protected:
        virtual void respond( YIELD::auto_Object<YIELD::ExceptionResponse> exception_response, YIELD::auto_EventTarget final_response_target )
        {
          final_response_target->send( *exception_response.release() );
        }

        virtual void respond( std::vector<YIELD::Event*>& responses, YIELD::auto_EventTarget final_response_target )
        {
          final_response_target->send( responses[responses.size() - 1]->incRef() );
        }

      private:
        size_t expected_response_count;
        YIELD::auto_EventTarget final_response_target;

        std::vector<YIELD::Event*> responses;
        YIELD::Mutex responses_lock;
      };
//
//      
//      class OSDWriteResponseTarget : public YIELD::EventTarget
//      {
//      public:
//        OSDWriteResponseTarget( YIELD::auto_Object<org::xtreemfs::interfaces::OSDInterface::writeRequest> write_request )
//          : original_response_target( write_request->get_response_target() ), write_request( write_request )
//        { }
//
//        // YIELD::Object
//        YIELD_OBJECT_PROTOTYPES( OSDWriteResponseTarget, 1 );
//
//        // YIELD::EventTarget
//        void send( YIELD::Event& ev )
//        {
//          if ( ev.get_tag() == YIELD_OBJECT_TAG( org::xtreemfs::interfaces::OSDInterface::writeResponse ) )
//            static_cast<org::xtreemfs::interfaces::OSDInterface::writeResponse&>( ev ).set_selected_file_replica( write_request->get_selected_file_replica() );
//          original_response_target->send( ev );
//        }
//
//      private:
//        YIELD::auto_EventTarget original_response_target;
//        YIELD::auto_Object<org::xtreemfs::interfaces::OSDInterface::writeRequest> write_request;
//      };
    };
  };
};


OSDProxyMux::OSDProxyMux( YIELD::auto_Object<DIRProxy> dir_proxy, uint32_t flags, YIELD::auto_Log log, const YIELD::Time& operation_timeout, YIELD::auto_SSLContext ssl_context )
  : dir_proxy( dir_proxy ), flags( flags ), log( log ), operation_timeout( operation_timeout ), ssl_context( ssl_context )
{
  get_osd_ping_interval_s = NULL;
  select_file_replica = NULL;

  std::vector<YIELD::Path> policy_dir_paths;
  policy_dir_paths.push_back( "policies" );
  policy_dir_paths.push_back( "lib" );
  policy_dir_paths.push_back( YIELD::Path() );
  YIELD::auto_Volume volume = new YIELD::Volume;
  for ( std::vector<YIELD::Path>::iterator policy_dir_path_i = policy_dir_paths.begin(); policy_dir_path_i != policy_dir_paths.end(); policy_dir_path_i++ )
  {
    std::vector<YIELD::Path> file_names;
    volume->listdir( *policy_dir_path_i, file_names );
    for ( std::vector<YIELD::Path>::iterator file_name_i = file_names.begin(); file_name_i != file_names.end(); file_name_i++ )
    {
      const std::string& file_name = static_cast<const std::string&>( *file_name_i );
      std::string::size_type dll_pos = file_name.find( SHLIBSUFFIX );
      if ( dll_pos != std::string::npos && dll_pos != 0 && file_name[dll_pos-1] == '.' )
      {
        YIELD::auto_Object<YIELD::SharedLibrary> policy_shared_library = YIELD::SharedLibrary::open( *file_name_i );
        if ( policy_shared_library != NULL )
        {
          get_osd_ping_interval_s = policy_shared_library->getFunction( "get_osd_ping_interval_s", get_osd_ping_interval_s );
          select_file_replica = policy_shared_library->getFunction( "select_file_replica", select_file_replica );
          policy_shared_libraries.push_back( policy_shared_library.release() );
        }
      }
    }
  }
}

OSDProxyMux::~OSDProxyMux()
{
  for ( OSDProxyMap::iterator osd_proxies_i = osd_proxies.begin(); osd_proxies_i != osd_proxies.end(); osd_proxies_i++ )
  {
    YIELD::Object::decRef( *osd_proxies_i->second.first );
    YIELD::Object::decRef( *osd_proxies_i->second.second );
  }
}

YIELD::auto_Object<OSDProxy> OSDProxyMux::getTCPOSDProxy( OSDProxyRequest& osd_proxy_request, const org::xtreemfs::interfaces::FileCredentials& file_credentials, uint64_t object_number )
{
  const org::xtreemfs::interfaces::ReplicaSet& file_replicas = file_credentials.get_xlocs().get_replicas();
  const org::xtreemfs::interfaces::Replica* selected_file_replica = NULL;

  if ( osd_proxy_request.get_selected_file_replica() != SIZE_MAX )
    selected_file_replica = &file_replicas[osd_proxy_request.get_selected_file_replica()];
  else
  {
    if ( file_replicas.size() == 1 || select_file_replica == NULL )
    {
      selected_file_replica = &file_replicas[0];
      osd_proxy_request.set_selected_file_replica( 0 );
    }
    else
    {
      struct file_replica_t* c_file_replicas = new file_replica_t[file_replicas.size()];
      for ( org::xtreemfs::interfaces::ReplicaSet::size_type file_replica_i = 0; file_replica_i < file_replicas.size(); file_replica_i++ )
      {
        const org::xtreemfs::interfaces::Replica& file_replica = file_replicas[file_replica_i];
        const org::xtreemfs::interfaces::StringSet& osd_uuids = file_replica.get_osd_uuids();
        struct file_replica_t& c_file_replica = c_file_replicas[file_replica_i];
        c_file_replica.striping_policy_type = file_replica.get_striping_policy().get_type();
        c_file_replica.osds = new osd_t[osd_uuids.size()];
        c_file_replica.osds_len = osd_uuids.size();
        for ( org::xtreemfs::interfaces::StringSet::size_type osd_i = 0; osd_i < osd_uuids.size(); osd_i++ )
        {
          struct osd_t& c_osd = c_file_replica.osds[osd_i];
          memset( &c_osd, 0, sizeof( c_osd ) );
          c_osd.uuid = osd_uuids[osd_i].c_str();
          OSDProxyMap::iterator osd_proxies_i = osd_proxies.find( osd_uuids[osd_i] );
          if ( osd_proxies_i != osd_proxies.end() )
          {
            OSDProxy* udp_osd_proxy = osd_proxies_i->second.second;
            if ( udp_osd_proxy != NULL )
            {
              c_osd.rtt_ms = static_cast<int>( udp_osd_proxy->get_rtt().as_unix_time_ms() );
              c_osd.x_coordinate = udp_osd_proxy->get_vivaldi_coordinates().get_x_coordinate();
              c_osd.y_coordinate = udp_osd_proxy->get_vivaldi_coordinates().get_y_coordinate();
              c_osd.local_error = udp_osd_proxy->get_vivaldi_coordinates().get_local_error();
            }
          }
        }
      }

      int selected_file_replica_i = select_file_replica( file_credentials.get_xcap().get_file_id().c_str(), file_credentials.get_xcap().get_access_mode(), c_file_replicas, file_replicas.size() );
      if ( selected_file_replica_i >= 0 )
      {
        selected_file_replica = &file_replicas[selected_file_replica_i];
        osd_proxy_request.set_selected_file_replica( selected_file_replica_i );
      }
      else
      {
        selected_file_replica = &file_replicas[0];
        osd_proxy_request.set_selected_file_replica( 0 );
      }

      for ( org::xtreemfs::interfaces::ReplicaSet::size_type file_replica_i = 0; file_replica_i < file_replicas.size(); file_replica_i++ )
        delete [] c_file_replicas[file_replica_i].osds;
      delete [] c_file_replicas;
    }
  }    

  const org::xtreemfs::interfaces::StripingPolicy& striping_policy = selected_file_replica->get_striping_policy();

  switch ( striping_policy.get_type() )
  {
    case org::xtreemfs::interfaces::STRIPING_POLICY_RAID0:
    {      
      size_t osd_i = object_number % striping_policy.get_width();
      const std::string& osd_uuid = selected_file_replica->get_osd_uuids()[osd_i];
      return getTCPOSDProxy( osd_uuid );
    }

    default: YIELD::DebugBreak(); throw YIELD::Exception(); break;
  }
}

YIELD::auto_Object<OSDProxy> OSDProxyMux::getTCPOSDProxy( const std::string& osd_uuid )
{
  OSDProxyMap::iterator osd_proxies_i = osd_proxies.find( osd_uuid );

  YIELD::auto_Object<OSDProxy> tcp_osd_proxy, udp_osd_proxy;

  if ( osd_proxies_i != osd_proxies.end() )
  {
    tcp_osd_proxy = osd_proxies_i->second.first->incRef();
    if ( osd_proxies_i->second.second != NULL )
      udp_osd_proxy = osd_proxies_i->second.second->incRef();
  }
  else
  {
    YIELD::auto_Object<org::xtreemfs::interfaces::AddressMappingSet> address_mappings = dir_proxy->getAddressMappingsFromUUID( osd_uuid );
    for ( org::xtreemfs::interfaces::AddressMappingSet::iterator address_mapping_i = address_mappings->begin(); address_mapping_i != address_mappings->end(); address_mapping_i++ )
    {
#ifdef YIELD_HAVE_OPENSSL
      if ( ssl_context != NULL && ( *address_mapping_i ).get_protocol() == org::xtreemfs::interfaces::ONCRPCS_SCHEME )
        tcp_osd_proxy = OSDProxy::create( ( *address_mapping_i ).get_uri(), osd_uuid, flags, log, operation_timeout, OSDProxy::PING_INTERVAL_DEFAULT, ssl_context ).release();
      else
#endif
      if ( ( *address_mapping_i ).get_protocol() == org::xtreemfs::interfaces::ONCRPC_SCHEME )
        tcp_osd_proxy = OSDProxy::create( ( *address_mapping_i ).get_uri(), osd_uuid, flags, log, operation_timeout, OSDProxy::PING_INTERVAL_DEFAULT, ssl_context ).release();
//      else if ( ( *address_mapping_i ).get_protocol() == org::xtreemfs::interfaces::ONCRPCU_SCHEME )
//        udp_osd_proxy = OSDProxy::create( ( *address_mapping_i ).get_uri(), stage_group, osd_uuid, flags, log, operation_timeout, OSDProxy::PING_INTERVAL_DEFAULT, ssl_context ).release();
    }

    if ( tcp_osd_proxy != NULL )
      osd_proxies[osd_uuid] = std::pair<OSDProxy*, OSDProxy*>( &tcp_osd_proxy->incRef(), YIELD::Object::incRef( udp_osd_proxy.get() ) );
    else
      throw YIELD::Exception( "no acceptable ONC-RPC URI for UUID" );
  }

  //if ( udp_osd_proxy != NULL )
  //  pingOSD( udp_osd_proxy );

  return tcp_osd_proxy;
}

//void OSDProxyMux::handleEvent( YIELD::Event& ev )
//{
//  switch ( ev.get_tag() )
//  {
//    case YIELD_OBJECT_TAG( OSDPingResponse ):
//    {
//      OSDPingResponse& ping_response = static_cast<OSDPingResponse&>( ev );
//
//      OSDProxyMap::iterator osd_proxies_i = osd_proxies.find( ping_response.get_target_osd_uuid() );
//      if ( osd_proxies_i != osd_proxies.end() )
//      {
//        OSDProxy* udp_osd_proxy = osd_proxies_i->second.second;
//        udp_osd_proxy->set_rtt( ping_response.get_rtt() );
//        udp_osd_proxy->set_vivaldi_coordinates( ping_response.get_remote_coordinates() );
//      }
//
//      YIELD::Object::decRef( ev );
//    }
//    break;
//
//    case YIELD_OBJECT_TAG( YIELD::FDEventQueue::TimerEvent ):
//    {
//      YIELD::auto_Object<OSDProxy> udp_osd_proxy = static_cast<OSDProxy*>( static_cast<YIELD::FDEventQueue::TimerEvent&>( ev ).get_context().release() );
//      pingOSD( udp_osd_proxy );
//      YIELD::Object::decRef( ev );
//    }
//    break;
//
//    default:
//    {
//      org::xtreemfs::interfaces::OSDInterface::handleEvent( ev );
//    }
//    break;
//  }
//}

void OSDProxyMux::handlereadRequest( readRequest& req )
{
  YIELD::auto_Object<OSDProxy> tcp_osd_proxy( getTCPOSDProxy( req, req.get_file_credentials(), req.get_object_number() ) );
  //if ( req.get_response_target()->get_tag() != YIELD_OBJECT_TAG( OSDReadResponseTarget ) )
  //  req.set_response_target( new OSDReadResponseTarget( req ) );
  static_cast<YIELD::EventTarget*>( tcp_osd_proxy.get() )->send( req );
}

void OSDProxyMux::handletruncateRequest( truncateRequest& req )
{
  const org::xtreemfs::interfaces::ReplicaSet& replicas = req.get_file_credentials().get_xlocs().get_replicas();

  if ( req.get_response_target() != NULL )
    req.set_response_target( new OSDTruncateResponseTarget( replicas.size(),  req.get_response_target() ) );

  for ( org::xtreemfs::interfaces::ReplicaSet::const_iterator replica_i = replicas.begin(); replica_i != replicas.end(); replica_i++ )
    static_cast<YIELD::EventTarget*>( getTCPOSDProxy( ( *replica_i ).get_osd_uuids()[0] ).get() )->send( req.incRef() );

  YIELD::Object::decRef( req );
}

void OSDProxyMux::handleunlinkRequest( unlinkRequest& req )
{
  const org::xtreemfs::interfaces::ReplicaSet& replicas = req.get_file_credentials().get_xlocs().get_replicas();

  if ( req.get_response_target() != NULL )
    req.set_response_target( new OSDTruncateResponseTarget( replicas.size(), req.get_response_target() ) );

  for ( org::xtreemfs::interfaces::ReplicaSet::const_iterator replica_i = replicas.begin(); replica_i != replicas.end(); replica_i++ )
    static_cast<YIELD::EventTarget*>( getTCPOSDProxy( ( *replica_i ).get_osd_uuids()[0] ).get() )->send( req.incRef() );

  YIELD::Object::decRef( req );
}

void OSDProxyMux::handlewriteRequest( writeRequest& req )
{
  YIELD::auto_Object<OSDProxy> tcp_osd_proxy( getTCPOSDProxy( req, req.get_file_credentials(), req.get_object_number() ) );
  //if ( req.get_response_target()->get_tag() != YIELD_OBJECT_TAG( OSDWriteResponseTarget ) )
  //  req.set_response_target( new OSDWriteResponseTarget( req ) );
  static_cast<YIELD::EventTarget*>( tcp_osd_proxy.get() )->send( req );
}

//void OSDProxyMux::pingOSD( YIELD::auto_Object<OSDProxy> udp_osd_proxy )
//{
//  if ( get_osd_ping_interval_s )
//  {
//    int osd_ping_interval_s = get_osd_ping_interval_s( udp_osd_proxy->get_uuid().c_str() );
//    YIELD::Time osd_ping_interval( osd_ping_interval_s * NS_IN_S );
//    udp_osd_proxy->set_ping_interval( osd_ping_interval );
////    if ( osd_ping_interval_s != 0 )
////      fd_event_queue->timer_create( osd_ping_interval, udp_osd_proxy.release() );
////    else
//      return;
//  }
//  else
//  {
//    udp_osd_proxy->set_ping_interval( YIELD::Time( static_cast<uint64_t>( 0 ) ) );
//    // return; 
//  }
//
//  org::xtreemfs::interfaces::OSDInterface::xtreemfs_pingRequest* ping_request = new org::xtreemfs::interfaces::OSDInterface::xtreemfs_pingRequest;
//  ping_request->set_response_target( new OSDPingResponseTarget( this->incRef(), udp_osd_proxy->get_uuid() ) );
//
//  static_cast<YIELD::EventTarget*>( udp_osd_proxy.get() )->send( *ping_request );
//}
