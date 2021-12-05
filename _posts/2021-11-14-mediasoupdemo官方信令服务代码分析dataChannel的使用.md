---
layout:     post
title:      mediasoupdemo官方信令服务代码分析dataChannel的使用
subtitle:   mediasoup、WebRTC、DataChannel
date:       2021-11-14
times:       16::44::28
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog: 	 true
tags:
    - mediasoup
    - WebRTC
    - 音视频
---




<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

# 前言


@[TOC](mediasoupdemo官方信令服务代码分析)

[mediasoup客户端推流代码地址：https://github.com/chensongpoixs/cmediasoupclient](https://github.com/chensongpoixs/cmediasoupclient)


[mediasoup服务器代码地址：https://github.com/chensongpoixs/cmediasoup](https://github.com/chensongpoixs/cmediasoup)




## 一、 登录流程分析
mediasoup的官方提供demo支持https和wss协议 
### 1、wss登录的流程
wss协议的登录请求 ：wss://127.0.0.1:3000/?roomId=chensong&peerId=xiqhlyrn


然后信令服务器接收新的客户端连接的处理获取roomId和peerId，判断roomId是否存在不存在就创建该房间的信息初始化当前房间中信息

getOrCreateRoom方法中判断room中是否有该房间没有创建房间，没有调用Room.create方法创建一个方法

在Room中create带有mediasoupWorker.createRouter创建一个管理房间Router用于管理

带有 mediasoupRouter.createAudioLevelObserver方法设置房间初始化音频基本信息

Bot.create


服务器的代码主要逻辑分别在server.js和room.js中


```javascript
protooWebSocketServer.on('connectionrequest', (info, accept, reject) =>
	{
		logger.debug('connectionrequest = ' + info.request.url);
		// The client indicates the roomId and peerId in the URL query.
		const u = url.parse(info.request.url, true);
		const roomId = u.query['roomId'];
		const peerId = u.query['peerId'];

		if (!roomId || !peerId)
		{
			reject(400, 'Connection request without roomId and/or peerId');

			return;
		}

		logger.info(
			'protoo connection request [roomId:%s, peerId:%s, address:%s, origin:%s]',
			roomId, peerId, info.socket.remoteAddress, info.origin);

		// Serialize this code into the queue to avoid that two peers connecting at
		// the same time with the same roomId create two separate rooms with same
		// roomId.
		queue.push(async () =>
		{
			// 查看是否有该房间没有就创建该房间
			const room = await getOrCreateRoom({ roomId });

			// Accept the protoo WebSocket connection.
			const protooWebSocketTransport = accept();

			room.handleProtooConnection({ peerId, protooWebSocketTransport });
		})
			.catch((error) =>
			{
				logger.error('room creation or room joining failed:%o', error);

				reject(error);
			});
	});
/**
 * Get a Room instance (or create one if it does not exist).
 */
async function getOrCreateRoom({ roomId })
{
	let room = rooms.get(roomId);

	// If the Room does not exist create a new one.
	if (!room)
	{
		logger.info('creating a new Room [roomId:%s]', roomId);

		const mediasoupWorker = getMediasoupWorker();
		// create room id mediasoup worker
		room = await Room.create({ mediasoupWorker, roomId });

		rooms.set(roomId, room);
		room.on('close', () => rooms.delete(roomId));
	}

	return room;
}
```




room.js create

```javascript
/**
	 * Factory function that creates and returns Room instance.
	 *
	 * @async
	 *
	 * @param {mediasoup.Worker} mediasoupWorker - The mediasoup Worker in which a new
	 *   mediasoup Router must be created.
	 * @param {String} roomId - Id of the Room instance.
	 */
	static async create({ mediasoupWorker, roomId })
	{
		logger.info('create() [roomId:%s]', roomId);

		// Create a protoo Room instance.
		const protooRoom = new protoo.Room();

		// Router media codecs.
		const { mediaCodecs } = config.mediasoup.routerOptions;

		logger.debug('mediacodecs = ' + mediaCodecs);
		// Create a mediasoup Router.
		const mediasoupRouter = await mediasoupWorker.createRouter({ mediaCodecs });

		// Create a mediasoup AudioLevelObserver.
		const audioLevelObserver = await mediasoupRouter.createAudioLevelObserver(
			{
				maxEntries : 1,
				threshold  : -80,
				interval   : 800
			});

		const bot = await Bot.create({ mediasoupRouter });

		return new Room(
			{
				roomId,
				protooRoom,
				mediasoupRouter,
				audioLevelObserver,
				bot
			});
	}


```

客户端代码在RoomClient中join的流程

```javascript
//浏览器登录流程 wss请求
async join()
	{
		//var ws_url = 'wss://127.0.0.1:3000/?roomId=chensong&peerId=xiqhlyrn';
		logger.debug('join url = ' + this._protooUrl);
		const protooTransport = new protooClient.WebSocketTransport(this._protooUrl);

		this._protoo = new protooClient.Peer(protooTransport);

		store.dispatch(
			stateActions.setRoomState('connecting'));

		this._protoo.on('open', () => this._joinRoom());

		this._protoo.on('failed', () =>
		{
			store.dispatch(requestActions.notify(
				{
					type : 'error',
					text : 'WebSocket connection failed'
				}));
		});

		this._protoo.on('disconnected', () =>
		{
			store.dispatch(requestActions.notify(
				{
					type : 'error',
					text : 'WebSocket disconnected'
				}));

			// Close mediasoup Transports.
			if (this._sendTransport)
			{
				this._sendTransport.close();
				this._sendTransport = null;
			}

			if (this._recvTransport)
			{
				this._recvTransport.close();
				this._recvTransport = null;
			}

			store.dispatch(
				stateActions.setRoomState('closed'));
		});

		this._protoo.on('close', () =>
		{
			if (this._closed)
				return;

			this.close();
		});
		//客户端请求协议发送格式
		// eslint-disable-next-line no-unused-vars
		this._protoo.on('request', async (request, accept, reject) =>
		{
			logger.debug(
				'proto "request" event [method:%s, data:%o]',
				request.method, request.data);
				var timestamp = (new Date()).valueOf();
			logger.debug('var timestamp = (new Date()).valueOf() = ' + timestamp);
			switch (request.method)
			{
				case 'newConsumer':
				{
					if (!this._consume)
					{
						reject(403, 'I do not want to consume');

						break;
					}

					const {
						peerId,
						producerId,
						id,
						kind,
						rtpParameters,
						type,
						appData,
						producerPaused
					} = request.data;

					try
					{
						const consumer = await this._recvTransport.consume(
							{
								id,
								producerId,
								kind,
								rtpParameters,
								appData : { ...appData, peerId } // Trick.
							});

						if (this._e2eKey && e2e.isSupported())
						{
							e2e.setupReceiverTransform(consumer.rtpReceiver);
						}

						// Store in the map.
						this._consumers.set(consumer.id, consumer);

						consumer.on('transportclose', () =>
						{
							this._consumers.delete(consumer.id);
						});

						const { spatialLayers, temporalLayers } =
							mediasoupClient.parseScalabilityMode(
								consumer.rtpParameters.encodings[0].scalabilityMode);

						store.dispatch(stateActions.addConsumer(
							{
								id                     : consumer.id,
								type                   : type,
								locallyPaused          : false,
								remotelyPaused         : producerPaused,
								rtpParameters          : consumer.rtpParameters,
								spatialLayers          : spatialLayers,
								temporalLayers         : temporalLayers,
								preferredSpatialLayer  : spatialLayers - 1,
								preferredTemporalLayer : temporalLayers - 1,
								priority               : 1,
								codec                  : consumer.rtpParameters.codecs[0].mimeType.split('/')[1],
								track                  : consumer.track
							},
							peerId));

						// We are ready. Answer the protoo request so the server will
						// resume this Consumer (which was paused for now if video).
						accept();

						// If audio-only mode is enabled, pause it.
						if (consumer.kind === 'video' && store.getState().me.audioOnly)
							this._pauseConsumer(consumer);
					}
					catch (error)
					{
						logger.error('"newConsumer" request failed:%o', error);

						store.dispatch(requestActions.notify(
							{
								type : 'error',
								text : `Error creating a Consumer: ${error}`
							}));

						throw error;
					}

					break;
				}
				// 服务器通知客户端 有新的需要消费的数据通道需要去连接一下哈
				case 'newDataConsumer':
				{
					if (!this._consume)
					{
						reject(403, 'I do not want to data consume');

						break;
					}

					if (!this._useDataChannel)
					{
						reject(403, 'I do not want DataChannels');

						break;
					}

					const {
						peerId, // NOTE: Null if bot.
						dataProducerId,
						id,
						sctpStreamParameters,
						label,
						protocol,
						appData
					} = request.data;

					try
					{
						const dataConsumer = await this._recvTransport.consumeData(
							{
								id,
								dataProducerId,
								sctpStreamParameters,
								label,
								protocol,
								appData : { ...appData, peerId } // Trick.
							});

						// Store in the map.
						this._dataConsumers.set(dataConsumer.id, dataConsumer);

						dataConsumer.on('transportclose', () =>
						{
							this._dataConsumers.delete(dataConsumer.id);
						});

						dataConsumer.on('open', () =>
						{
							logger.debug('DataConsumer "open" event');
						});

						dataConsumer.on('close', () =>
						{
							logger.warn('DataConsumer "close" event');

							this._dataConsumers.delete(dataConsumer.id);

							store.dispatch(requestActions.notify(
								{
									type : 'error',
									text : 'DataConsumer closed'
								}));
						});

						dataConsumer.on('error', (error) =>
						{
							logger.error('DataConsumer "error" event:%o', error);

							store.dispatch(requestActions.notify(
								{
									type : 'error',
									text : `DataConsumer error: ${error}`
								}));
						});

						dataConsumer.on('message', (message) =>
						{
							logger.debug(
								'DataConsumer "message" event [streamId:%d]',
								dataConsumer.sctpStreamParameters.streamId);

							// TODO: For debugging.
							window.DC_MESSAGE = message;

							if (message instanceof ArrayBuffer)
							{
								const view = new DataView(message);
								const number = view.getUint32();

								if (number == Math.pow(2, 32) - 1)
								{
									logger.warn('dataChannelTest finished!');

									this._nextDataChannelTestNumber = 0;

									return;
								}

								if (number > this._nextDataChannelTestNumber)
								{
									logger.warn(
										'dataChannelTest: %s packets missing',
										number - this._nextDataChannelTestNumber);
								}

								this._nextDataChannelTestNumber = number + 1;

								return;
							}
							else if (typeof message !== 'string')
							{
								logger.warn('ignoring DataConsumer "message" (not a string)');

								return;
							}

							switch (dataConsumer.label)
							{
								case 'chat':
								{
									const { peers } = store.getState();
									const peersArray = Object.keys(peers)
										.map((_peerId) => peers[_peerId]);
									const sendingPeer = peersArray
										.find((peer) => peer.dataConsumers.includes(dataConsumer.id));

									if (!sendingPeer)
									{
										logger.warn('DataConsumer "message" from unknown peer');

										break;
									}

									store.dispatch(requestActions.notify(
										{
											title   : `${sendingPeer.displayName} says:`,
											text    : message,
											timeout : 5000
										}));

									break;
								}

								case 'bot':
								{
									store.dispatch(requestActions.notify(
										{
											title   : 'Message from Bot:',
											text    : message,
											timeout : 5000
										}));

									break;
								}
							}
						});

						// TODO: REMOVE
						window.DC = dataConsumer;

						store.dispatch(stateActions.addDataConsumer(
							{
								id                   : dataConsumer.id,
								sctpStreamParameters : dataConsumer.sctpStreamParameters,
								label                : dataConsumer.label,
								protocol             : dataConsumer.protocol
							},
							peerId));

						// We are ready. Answer the protoo request.
						accept();
					}
					catch (error)
					{
						logger.error('"newDataConsumer" request failed:%o', error);

						store.dispatch(requestActions.notify(
							{
								type : 'error',
								text : `Error creating a DataConsumer: ${error}`
							}));

						throw error;
					}

					break;
				}
			}
		});
		// 信令服务器通知客户端的事件
		this._protoo.on('notification', (notification) =>
		{
			logger.debug(
				'proto "notification" event [method:%s, data:%o]',
				notification.method, notification.data);

			switch (notification.method)
			{
				case 'producerScore':
				{
					const { producerId, score } = notification.data;

					store.dispatch(
						stateActions.setProducerScore(producerId, score));

					break;
				}
				// 有新的客户端连接通知当前客户端新客户端连接的信息
				case 'newPeer':
				{
					const peer = notification.data;

					store.dispatch(
						stateActions.addPeer(
							{ ...peer, consumers: [], dataConsumers: [] }));

					store.dispatch(requestActions.notify(
						{
							text : `${peer.displayName} has joined the room`
						}));

					break;
				}
				// 有客户端退出通知客户端
				case 'peerClosed':
				{
					const { peerId } = notification.data;

					store.dispatch(
						stateActions.removePeer(peerId));

					break;
				}

				case 'peerDisplayNameChanged':
				{
					const { peerId, displayName, oldDisplayName } = notification.data;

					store.dispatch(
						stateActions.setPeerDisplayName(displayName, peerId));

					store.dispatch(requestActions.notify(
						{
							text : `${oldDisplayName} is now ${displayName}`
						}));

					break;
				}

				case 'downlinkBwe':
				{
					logger.debug('\'downlinkBwe\' event:%o', notification.data);

					break;
				}

				case 'consumerClosed':
				{
					const { consumerId } = notification.data;
					const consumer = this._consumers.get(consumerId);

					if (!consumer)
						break;

					consumer.close();
					this._consumers.delete(consumerId);

					const { peerId } = consumer.appData;

					store.dispatch(
						stateActions.removeConsumer(consumerId, peerId));

					break;
				}

				case 'consumerPaused':
				{
					const { consumerId } = notification.data;
					const consumer = this._consumers.get(consumerId);

					if (!consumer)
						break;

					consumer.pause();

					store.dispatch(
						stateActions.setConsumerPaused(consumerId, 'remote'));

					break;
				}

				case 'consumerResumed':
				{
					const { consumerId } = notification.data;
					const consumer = this._consumers.get(consumerId);

					if (!consumer)
						break;

					consumer.resume();

					store.dispatch(
						stateActions.setConsumerResumed(consumerId, 'remote'));

					break;
				}

				case 'consumerLayersChanged':
				{
					const { consumerId, spatialLayer, temporalLayer } = notification.data;
					const consumer = this._consumers.get(consumerId);

					if (!consumer)
						break;

					store.dispatch(stateActions.setConsumerCurrentLayers(
						consumerId, spatialLayer, temporalLayer));

					break;
				}

				case 'consumerScore':
				{
					const { consumerId, score } = notification.data;

					store.dispatch(
						stateActions.setConsumerScore(consumerId, score));

					break;
				}

				case 'dataConsumerClosed':
				{
					const { dataConsumerId } = notification.data;
					const dataConsumer = this._dataConsumers.get(dataConsumerId);

					if (!dataConsumer)
						break;

					dataConsumer.close();
					this._dataConsumers.delete(dataConsumerId);

					const { peerId } = dataConsumer.appData;

					store.dispatch(
						stateActions.removeDataConsumer(dataConsumerId, peerId));

					break;
				}

				case 'activeSpeaker':
				{
					const { peerId } = notification.data;

					store.dispatch(
						stateActions.setRoomActiveSpeaker(peerId));

					break;
				}

				default:
				{
					logger.error(
						'unknown protoo notification.method "%s"', notification.method);
				}
			}
		});
	}
```


### 2、 https的流程


http协议是个C++客户端方便写客户端代码而不需要写一些websocket代码， 可以解决客户端开发难度使用websocket开发C++客户端推流难度是有点麻烦 而使用http协议就会变的简单多了


```javascript
/**
 * Create a Node.js HTTPS server. It listens in the IP and port given in the
 * configuration file and reuses the Express application as request listener.
 */
async function runHttpsServer()
{
	logger.info('running an HTTPS server...');

	// HTTPS server for the protoo WebSocket server.
	const tls =
	{
		cert : fs.readFileSync(config.https.tls.cert),
		key  : fs.readFileSync(config.https.tls.key)
	};

	httpsServer = https.createServer(tls, expressApp);
	
	await new Promise((resolve) =>
	{
		httpsServer.listen(
			Number(config.https.listenPort), config.https.listenIp, resolve);
	});
	
	
	// 增加http协议是C++客户端登录创建webrtc一些请求方法
	httpServer = http.createServer(expressApp);
	await new Promise((resolve) =>
	{
		httpServer.listen(8888, '0.0.0.0', resolve);
	});
}
/**
 * Create an Express based API server to manage Broadcaster requests.
 */
async function createExpressApp()
{
	logger.info('creating Express app...');

	expressApp = express();

	expressApp.use(bodyParser.json());

	/**
	 * For every API request, verify that the roomId in the path matches and
	 * existing room.
	 */
	expressApp.param(
		'roomId', (req, res, next, roomId) =>
		{
			// The room must exist for all API requests.
			if (!rooms.has(roomId))
			{
				const error = new Error(`room with id "${roomId}" not found`);

				error.status = 404;
				throw error;
			}

			req.room = rooms.get(roomId);

			next();
		});

	/**
	 * API GET resource that returns the mediasoup Router RTP capabilities of
	 * the room.
	 */
	expressApp.get(
		'/rooms/:roomId', (req, res) =>
		{
			logger.error('=============/rooms/:roomId');
			const data = req.room.getRouterRtpCapabilities();

			res.status(200).json(data);
		});
	// 获取房间中所有人的producerId和
	expressApp.get(
	'/rooms/:roomId/chensong', (req, res) =>
	{
		//logger.error('getproducedata  =============/rooms/:roomId/getproducedata');
		

		const data = req.room.getAllPeerProduceData();

		res.status(200).send(data);

		//res.status(200).json(data);
	});
	/**
	 * POST API to create a Broadcaster.
	 */
	expressApp.post(
		'/rooms/:roomId/broadcasters', async (req, res, next) =>
		{
			logger.error('=============/rooms/:roomId/broadcasters');
			logger.info('req =' + req.body);
			const {
				id,
				displayName,
				device,
				rtpCapabilities
			} = req.body;

			try
			{
				const data = await req.room.createBroadcaster(
					{
						id,
						displayName,
						device,
						rtpCapabilities
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next("chenosng test error !!!!");
			}
		});

	/**
	 * DELETE API to delete a Broadcaster.
	 */
	expressApp.delete(
		'/rooms/:roomId/broadcasters/:broadcasterId', (req, res) =>
		{
			logger.error('=============/rooms/:roomId/broadcasters/:broadcasterId');
			logger.info('req =' + req.body);
			const { broadcasterId } = req.params;

			req.room.deleteBroadcaster({ broadcasterId });

			res.status(200).send('broadcaster deleted');
		});
		
	expressApp.get(
		'/rooms/:roomId/broadcasters/:broadcasterId', (req, res) =>
		{
			logger.error('delete =============/rooms/:roomId/broadcasters/:broadcasterId');
			const { broadcasterId } = req.params;

			req.room.deleteBroadcaster({ broadcasterId });

			res.status(200).send('broadcaster deleted');

			//res.status(200).json(data);
		});
	
	/**
	 * POST API to create a mediasoup Transport associated to a Broadcaster.
	 * It can be a PlainTransport or a WebRtcTransport depending on the
	 * type parameters in the body. There are also additional parameters for
	 * PlainTransport.
	 */
	expressApp.post(
		'/rooms/:roomId/broadcasters/:broadcasterId/transports',
		async (req, res, next) =>
		{
			logger.error('=============/rooms/:roomId/broadcasters/:broadcasterId/transports');
			logger.info('req =' + req.body);
			const { broadcasterId } = req.params;
			const { type, rtcpMux, comedia, sctpCapabilities } = req.body;

			try
			{
				const data = await req.room.createBroadcasterTransport(
					{
						broadcasterId,
						type,
						rtcpMux,
						comedia, 
						sctpCapabilities
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next(error);
			}
		});

	/**
	 * POST API to connect a Transport belonging to a Broadcaster. Not needed
	 * for PlainTransport if it was created with comedia option set to true.
	 */
	expressApp.post(
		'/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/connect',
		async (req, res, next) =>
		{
			logger.error('=============/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/connect');
			logger.info('req =' + req.body);
			const { broadcasterId, transportId } = req.params;
			const { dtlsParameters } = req.body;

			try
			{
				const data = await req.room.connectBroadcasterTransport(
					{
						broadcasterId,
						transportId,
						dtlsParameters
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next(error);
			}
		});

	/**
	 * POST API to create a mediasoup Producer associated to a Broadcaster.
	 * The exact Transport in which the Producer must be created is signaled in
	 * the URL path. Body parameters include kind and rtpParameters of the
	 * Producer.
	 */
	expressApp.post(
		'/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/producers',
		async (req, res, next) =>
		{
			logger.error('===/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/producers');
			logger.info('req =' + req.body);
			const { broadcasterId, transportId } = req.params;
			const { kind, rtpParameters } = req.body;

			try
			{
				const data = await req.room.createBroadcasterProducer(
					{
						broadcasterId,
						transportId,
						kind,
						rtpParameters
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next(error);
			}
		});

	/**
	 * POST API to create a mediasoup Consumer associated to a Broadcaster.
	 * The exact Transport in which the Consumer must be created is signaled in
	 * the URL path. Query parameters must include the desired producerId to
	 * consume.
	 */
	expressApp.post(
		'/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/consume',
		async (req, res, next) =>
		{
			logger.error('===/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/consume');
			logger.info('req =' + req.body);
			const { broadcasterId, transportId } = req.params;
			const { producerId } = req.query;

			try
			{
				const data = await req.room.createBroadcasterConsumer(
					{
						broadcasterId,
						transportId,
						producerId
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next(error);
			}
		});

	/**
	 * POST API to create a mediasoup DataConsumer associated to a Broadcaster.
	 * The exact Transport in which the DataConsumer must be created is signaled in
	 * the URL path. Query body must include the desired producerId to
	 * consume.
	 */
	expressApp.post(
		'/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/consume/data',
		async (req, res, next) =>
		{
			logger.error('===/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/consume/data');
			logger.info('req =' + req.body);
			const { broadcasterId, transportId } = req.params;
			const { dataProducerId } = req.body;

			try
			{
				const data = await req.room.createBroadcasterDataConsumer(
					{
						broadcasterId,
						transportId,
						dataProducerId
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next(error);
			}
		});
	
	expressApp.post(
		'/rooms/:roomId/peerId/:peerId/transports/:transportId/consume/data',
		async (req, res, next) =>
		{
			logger.error('===/rooms/:roomId/peerId/:peerId/transports/:transportId/consume/data');
			logger.info('req =' + req.body);
			const { peerId, transportId } = req.params;
			const { dataProducerId } = req.body;

			try
			{
				const data = await req.room.createBroadcasterDataConsumer(
					{
						peerId,
						transportId,
						dataProducerId
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next(error);
			}
		});
	/**
	 * POST API to create a mediasoup DataProducer associated to a Broadcaster.
	 * The exact Transport in which the DataProducer must be created is signaled in
	 */
	expressApp.post(
		'/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/produce/data',
		async (req, res, next) =>
		{
			logger.error('===/rooms/:roomId/broadcasters/:broadcasterId/transports/:transportId/produce/data');
			logger.info('req =' + req.body);
			const { broadcasterId, transportId } = req.params;
			const { label, protocol, sctpStreamParameters, appData } = req.body;

			try
			{
				const data = await req.room.createBroadcasterDataProducer(
					{
						broadcasterId,
						transportId,
						label,
						protocol,
						sctpStreamParameters,
						appData
					});

				res.status(200).json(data);
			}
			catch (error)
			{
				next(error);
			}
		});

	/**
	 * Error handler.
	 */
	expressApp.use(
		(error, req, res, next) =>
		{
			if (error)
			{
				logger.warn('Express app %s', String(error));

				error.status = error.status || (error.name === 'TypeError' ? 400 : 500);

				res.statusMessage = error.message;
				res.status(error.status).send(String(error));
			}
			else
			{
				next();
			}
		});
}

```
## 二、 join流程的分析

玩家加房间首先就改变自己的状态

然后把自己增加当前房间中所有的生产信息添加消费中去
订阅房间其他人的生产媒体信息和数据信息

客户端会同步接收newConsumer信息去使用webrtc底层连接该通道

客户端连接await this._recvTransport.consume(

DataConsume和上面的步骤是一样的

然后通知客户端有客户端加入该房间newPeer


```javascript
// Ensure the Peer is not already joined.
				if (peer.data.joined)
					throw new Error('Peer already joined');

				const {
					displayName,
					device,
					rtpCapabilities,
					sctpCapabilities
				} = request.data;

				// Store client data into the protoo Peer data object.
				peer.data.joined = true;
				peer.data.displayName = displayName;
				peer.data.device = device;
				peer.data.rtpCapabilities = rtpCapabilities;
				peer.data.sctpCapabilities = sctpCapabilities;

				// Tell the new Peer about already joined Peers.
				// And also create Consumers for existing Producers.
				// 房间所有房间包括广播的玩家
				const joinedPeers =
				[
					...this._getJoinedPeers(),
					...this._broadcasters.values()
				];

				// Reply now the request with the list of joined peers (all but the new one).
				const peerInfos = joinedPeers
					.filter((joinedPeer) => joinedPeer.id !== peer.id)
					.map((joinedPeer) => ({
						id          : joinedPeer.id,
						displayName : joinedPeer.data.displayName,
						device      : joinedPeer.data.device
					}));
				
				accept({ peers: peerInfos });

				// Mark the new Peer as joined.
				peer.data.joined = true;

				for (const joinedPeer of joinedPeers)
				{
				//订阅房间其他人的生产媒体信息和数据信息
					// Create Consumers for existing Producers.
					for (const producer of joinedPeer.data.producers.values())
					{
						this._createConsumer(
							{
								consumerPeer : peer,
								producerPeer : joinedPeer,
								producer
							});
					}

					// Create DataConsumers for existing DataProducers.
					for (const dataProducer of joinedPeer.data.dataProducers.values())
					{
						if (dataProducer.label === 'bot')
							continue;

						this._createDataConsumer(
							{
								dataConsumerPeer : peer,
								dataProducerPeer : joinedPeer,
								dataProducer
							});
					}
				}

				// Create DataConsumers for bot DataProducer.
				this._createDataConsumer(
					{
						dataConsumerPeer : peer,
						dataProducerPeer : null,
						dataProducer     : this._bot.dataProducer
					});

				// Notify the new Peer to all other Peers.
				for (const otherPeer of this._getJoinedPeers({ excludePeer: peer }))
				{
					otherPeer.notify(
						'newPeer',
						{
							id          : peer.id,
							displayName : peer.data.displayName,
							device      : peer.data.device
						})
						.catch(() => {});
				}

				break;
···

_createConsumer方法

```javascript
/**
	 * Creates a mediasoup Consumer for the given mediasoup Producer.
	 *
	 * @async
	 */
	async _createConsumer({ consumerPeer, producerPeer, producer })
	{
		// Optimization:
		// - Create the server-side Consumer in paused mode.
		// - Tell its Peer about it and wait for its response.
		// - Upon receipt of the response, resume the server-side Consumer.
		// - If video, this will mean a single key frame requested by the
		//   server-side Consumer (when resuming it).
		// - If audio (or video), it will avoid that RTP packets are received by the
		//   remote endpoint *before* the Consumer is locally created in the endpoint
		//   (and before the local SDP O/A procedure ends). If that happens (RTP
		//   packets are received before the SDP O/A is done) the PeerConnection may
		//   fail to associate the RTP stream.

		// NOTE: Don't create the Consumer if the remote Peer cannot consume it.
		if (
			!consumerPeer.data.rtpCapabilities ||
			!this._mediasoupRouter.canConsume(
				{
					producerId      : producer.id,
					rtpCapabilities : consumerPeer.data.rtpCapabilities
				})
		)
		{
			return;
		}

		// Must take the Transport the remote Peer is using for consuming.
		const transport = Array.from(consumerPeer.data.transports.values())
			.find((t) => t.appData.consuming);

		// This should not happen.
		if (!transport)
		{
			logger.warn('_createConsumer() | Transport for consuming not found');

			return;
		}

		// Create the Consumer in paused mode.
		let consumer;

		try
		{
			consumer = await transport.consume(
				{
					producerId      : producer.id,
					rtpCapabilities : consumerPeer.data.rtpCapabilities,
					paused          : true
				});
		}
		catch (error)
		{
			logger.warn('_createConsumer() | transport.consume():%o', error);

			return;
		}

		// Store the Consumer into the protoo consumerPeer data Object.
		consumerPeer.data.consumers.set(consumer.id, consumer);

		// Set Consumer events.
		consumer.on('transportclose', () =>
		{
			// Remove from its map.
			consumerPeer.data.consumers.delete(consumer.id);
		});

		consumer.on('producerclose', () =>
		{
			// Remove from its map.
			consumerPeer.data.consumers.delete(consumer.id);

			consumerPeer.notify('consumerClosed', { consumerId: consumer.id })
				.catch(() => {});
		});

		consumer.on('producerpause', () =>
		{
			consumerPeer.notify('consumerPaused', { consumerId: consumer.id })
				.catch(() => {});
		});

		consumer.on('producerresume', () =>
		{
			consumerPeer.notify('consumerResumed', { consumerId: consumer.id })
				.catch(() => {});
		});

		consumer.on('score', (score) =>
		{
			// logger.debug(
			// 	'consumer "score" event [consumerId:%s, score:%o]',
			// 	consumer.id, score);

			consumerPeer.notify('consumerScore', { consumerId: consumer.id, score })
				.catch(() => {});
		});

		consumer.on('layerschange', (layers) =>
		{
			consumerPeer.notify(
				'consumerLayersChanged',
				{
					consumerId    : consumer.id,
					spatialLayer  : layers ? layers.spatialLayer : null,
					temporalLayer : layers ? layers.temporalLayer : null
				})
				.catch(() => {});
		});

		// NOTE: For testing.
		// await consumer.enableTraceEvent([ 'rtp', 'keyframe', 'nack', 'pli', 'fir' ]);
		// await consumer.enableTraceEvent([ 'pli', 'fir' ]);
		// await consumer.enableTraceEvent([ 'keyframe' ]);

		consumer.on('trace', (trace) =>
		{
			logger.debug(
				'consumer "trace" event [producerId:%s, trace.type:%s, trace:%o]',
				consumer.id, trace.type, trace);
		});

		// Send a protoo request to the remote Peer with Consumer parameters.
		try
		{
			await consumerPeer.request(
				'newConsumer',
				{
					peerId         : producerPeer.id,
					producerId     : producer.id,
					id             : consumer.id,
					kind           : consumer.kind,
					rtpParameters  : consumer.rtpParameters,
					type           : consumer.type,
					appData        : producer.appData,
					producerPaused : consumer.producerPaused
				});

			// Now that we got the positive response from the remote endpoint, resume
			// the Consumer so the remote endpoint will receive the a first RTP packet
			// of this new stream once its PeerConnection is already ready to process
			// and associate it.
			await consumer.resume();

			consumerPeer.notify(
				'consumerScore',
				{
					consumerId : consumer.id,
					score      : consumer.score
				})
				.catch(() => {});
		}
		catch (error)
		{
			logger.warn('_createConsumer() | failed:%o', error);
		}
	}

```


RoomClient.js 

```javascript
	if (!this._consume)
					{
						reject(403, 'I do not want to consume');

						break;
					}

					const {
						peerId,
						producerId,
						id,
						kind,
						rtpParameters,
						type,
						appData,
						producerPaused
					} = request.data;

					try
					{
						const consumer = await this._recvTransport.consume(
							{
								id,
								producerId,
								kind,
								rtpParameters,
								appData : { ...appData, peerId } // Trick.
							});

						if (this._e2eKey && e2e.isSupported())
						{
							e2e.setupReceiverTransform(consumer.rtpReceiver);
						}

						// Store in the map.
						this._consumers.set(consumer.id, consumer);

						consumer.on('transportclose', () =>
						{
							this._consumers.delete(consumer.id);
						});

						const { spatialLayers, temporalLayers } =
							mediasoupClient.parseScalabilityMode(
								consumer.rtpParameters.encodings[0].scalabilityMode);

						store.dispatch(stateActions.addConsumer(
							{
								id                     : consumer.id,
								type                   : type,
								locallyPaused          : false,
								remotelyPaused         : producerPaused,
								rtpParameters          : consumer.rtpParameters,
								spatialLayers          : spatialLayers,
								temporalLayers         : temporalLayers,
								preferredSpatialLayer  : spatialLayers - 1,
								preferredTemporalLayer : temporalLayers - 1,
								priority               : 1,
								codec                  : consumer.rtpParameters.codecs[0].mimeType.split('/')[1],
								track                  : consumer.track
							},
							peerId));

						// We are ready. Answer the protoo request so the server will
						// resume this Consumer (which was paused for now if video).
						accept();

						// If audio-only mode is enabled, pause it.
						if (consumer.kind === 'video' && store.getState().me.audioOnly)
							this._pauseConsumer(consumer);
					}
					catch (error)
					{
						logger.error('"newConsumer" request failed:%o', error);

						store.dispatch(requestActions.notify(
							{
								type : 'error',
								text : `Error creating a Consumer: ${error}`
							}));

						throw error;
					}

					break;
				}

```

## 三、createwebrtcTransport的流程

## 四、connectwebrtcTransport的流程


## 五、使用webrtc中的DataChannel进行聊天（待续...）

使用DataChannel进行聊天鸭 ！！！！    



![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2021-11-14/mediasoupDataChannel.gif?raw=true)



# 总结