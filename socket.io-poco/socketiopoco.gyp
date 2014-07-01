{
	'targets': 
	[{
		'target_name': 'socketiopoco',
		'type': 'static_library',
		'include_dirs': 
		[
			'./include',
		],
		'sources': 
		[
			'src/SIOClient.cpp',
			'src/SIOClientImpl.cpp',
			'src/SIOClientRegistry.cpp',
			'src/SIOEventRegistry.cpp',
			'src/SIONotificationHandler.cpp',
			'src/SIOPacket.cpp',
		],
		'direct_dependent_settings': 
		{
			'include_dirs': 
			[
				'./include',
			],
			'conditions': 
			[
				[ 'os_posix == 1 and OS != "mac" and OS != "ios"', 
					{
						'cflags': [
						'-Wno-deprecated-declarations',
						],
						'cflags_cc': [
						'-frtti',
						'-fexceptions',
						],
					}
				],
			]
		},
		'link_settings': 
		{
			'libraries':
			[
				'/usr/local/lib/libPocoFoundation.so',
				'/usr/local/lib/libPocoNet.so',
				'/usr/local/lib/libPocoNetSSL.so',
				'/usr/local/lib/libPocoJSON.so',  
				'/usr/local/lib/libPocoUtil.so',   
			],
		},
		'conditions': 
		[
			[ 'os_posix == 1 and OS != "mac" and OS != "ios"', 
				{
					'cflags': [
					'-Wno-deprecated-declarations',
					],
					'cflags_cc': [
					'-frtti',
					'-fexceptions',
					],
				}
			],
		]
	},
	{
      'target_name': 'socketiotest',
      'type': 'executable',
      'dependencies': 
      [
        'socketiopoco',
        '<(DEPTH)/third_party/jsoncpp/jsoncpp.gyp:jsoncpp',
      ],
      'defines':
      [
      	'JSONCPP_RELATIVE_PATH',
      ],      
      'include_dirs':
      [
      	'../../../../',
      ],      	
      'sources': 
      [
      	'test/main.cpp',
      	'test/TestEndpointTarget.cpp',
      	'test/TestTarget.cpp',
      ],
    },  # target socketiotest
	],  
}