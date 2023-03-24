/*
 * tradermsg.cpp
 *
 * Copyright (c) 2022, 2023 Xiongfei Shi
 *
 * Author: Xiongfei Shi <xiongfei.shi(a)icloud.com>
 * License: Apache-2.0
 *
 * https://github.com/shixiongfei/napi-ctp
 */

#include "tradermsg.h"
#include "traderspi.h"
#include "ctpmsg.h"

static const MessageFuncs messageFuncs = {
  {ET_QUIT,                                     msgQuit},
  {ET_FRONTCONNECTED,                           msgFrontConnected},
  {ET_FRONTDISCONNECTED,                        msgFrontDisconnected},
  {ET_HEARTBEATWARNING,                         msgHeartBeatWarning},
  {ET_RSPAUTHENTICATE,                          msgAuthenticate},
  {ET_RSPUSERLOGIN,                             msgUserLogin},
  {ET_RSPUSERLOGOUT,                            msgUserLogout},
  {ET_RSPUSERPASSWORDUPDATE,                    msgUserPasswordUpdate},
  {ET_RSPTRADINGACCOUNTPASSWORDUPDATE,          msgTradingAccountPasswordUpdate},
  {ET_RSPUSERAUTHMETHOD,                        msgUserAuthMethod},
  {ET_RSPGENUSERCAPTCHA,                        msgGenUserCaptcha},
  {ET_RSPGENUSERTEXT,                           msgGenUserText},
  {ET_RSPORDERINSERT,                           msgOrderInsert},
  {ET_RSPPARKEDORDERINSERT,                     msgParkedOrderInsert},
  {ET_RSPPARKEDORDERACTION,                     msgParkedOrderAction},
  {ET_RSPORDERACTION,                           msgOrderAction},
  {ET_RSPQRYMAXORDERVOLUME,                     msgQryMaxOrderVolume},
  {ET_RSPSETTLEMENTINFOCONFIRM,                 msgSettlementInfoConfirm},
  {ET_RSPREMOVEPARKEDORDER,                     msgRemoveParkedOrder},
  {ET_RSPREMOVEPARKEDORDERACTION,               msgRemoveParkedOrderAction},
  {ET_RSPEXECORDERINSERT,                       msgExecOrderInsert},
  {ET_RSPEXECORDERACTION,                       msgExecOrderAction},
  {ET_RSPFORQUOTEINSERT,                        msgForQuoteInsert},
  {ET_RSPQUOTEINSERT,                           msgQuoteInsert},
  {ET_RSPQUOTEACTION,                           msgQuoteAction},
  {ET_RSPBATCHORDERACTION,                      msgBatchOrderAction},
  {ET_RSPOPTIONSELFCLOSEINSERT,                 msgOptionSelfCloseInsert},
  {ET_RSPOPTIONSELFCLOSEACTION,                 msgOptionSelfCloseAction},
  {ET_RSPCOMBACTIONINSERT,                      msgCombActionInsert},
  {ET_RSPQRYORDER,                              msgQryOrder},
  {ET_RSPQRYTRADE,                              msgQryTrade},
  {ET_RSPQRYINVESTORPOSITION,                   msgQryInvestorPosition},
  {ET_RSPQRYTRADINGACCOUNT,                     msgQryTradingAccount},
  {ET_RSPQRYINVESTOR,                           msgQryInvestor},
  {ET_RSPQRYTRADINGCODE,                        msgQryTradingCode},
  {ET_RSPQRYINSTRUMENTMARGINRATE,               msgQryInstrumentMarginRate},
  {ET_RSPQRYINSTRUMENTCOMMISSIONRATE,           msgQryInstrumentCommissionRate},
  {ET_RSPQRYEXCHANGE,                           msgQryExchange},
  {ET_RSPQRYPRODUCT,                            msgQryProduct},
  {ET_RSPQRYINSTRUMENT,                         msgQryInstrument},
  {ET_RSPQRYDEPTHMARKETDATA,                    msgQryDepthMarketData},
  {ET_RSPQRYTRADEROFFER,                        msgQryTraderOffer},
  {ET_RSPQRYSETTLEMENTINFO,                     msgQrySettlementInfo},
  {ET_RSPQRYTRANSFERBANK,                       msgQryTransferBank},
  {ET_RSPQRYINVESTORPOSITIONDETAIL,             msgQryInvestorPositionDetail},
  {ET_RSPQRYNOTICE,                             msgQryNotice},
  {ET_RSPQRYSETTLEMENTINFOCONFIRM,              msgQrySettlementInfoConfirm},
  {ET_RSPQRYINVESTORPOSITIONCOMBINEDETAIL,      msgQryInvestorPositionCombineDetail},
  {ET_RSPQRYCFMMCTRADINGACCOUNTKEY,             msgQryCFMMCTradingAccountKey},
  {ET_RSPQRYEWARRANTOFFSET,                     msgQryEWarrantOffset},
  {ET_RSPQRYINVESTORPRODUCTGROUPMARGIN,         msgQryInvestorProductGroupMargin},
  {ET_RSPQRYEXCHANGEMARGINRATE,                 msgQryExchangeMarginRate},
  {ET_RSPQRYEXCHANGEMARGINRATEADJUST,           msgQryExchangeMarginRateAdjust},
  {ET_RSPQRYEXCHANGERATE,                       msgQryExchangeRate},
  {ET_RSPQRYSECAGENTACIDMAP,                    msgQrySecAgentACIDMap},
  {ET_RSPQRYPRODUCTEXCHRATE,                    msgQryProductExchRate},
  {ET_RSPQRYPRODUCTGROUP,                       msgQryProductGroup},
  {ET_RSPQRYMMINSTRUMENTCOMMISSIONRATE,         msgQryMMInstrumentCommissionRate},
  {ET_RSPQRYMMOPTIONINSTRCOMMRATE,              msgQryMMOptionInstrCommRate},
  {ET_RSPQRYINSTRUMENTORDERCOMMRATE,            msgQryInstrumentOrderCommRate},
  {ET_RSPQRYSECAGENTTRADINGACCOUNT,             msgQrySecAgentTradingAccount},
  {ET_RSPQRYSECAGENTCHECKMODE,                  msgQrySecAgentCheckMode},
  {ET_RSPQRYSECAGENTTRADEINFO,                  msgQrySecAgentTradeInfo},
  {ET_RSPQRYOPTIONINSTRTRADECOST,               msgQryOptionInstrTradeCost},
  {ET_RSPQRYOPTIONINSTRCOMMRATE,                msgQryOptionInstrCommRate},
  {ET_RSPQRYEXECORDER,                          msgQryExecOrder},
  {ET_RSPQRYFORQUOTE,                           msgQryForQuote},
  {ET_RSPQRYQUOTE,                              msgQryQuote},
  {ET_RSPQRYOPTIONSELFCLOSE,                    msgQryOptionSelfClose},
  {ET_RSPQRYINVESTUNIT,                         msgQryInvestUnit},
  {ET_RSPQRYCOMBINSTRUMENTGUARD,                msgQryCombInstrumentGuard},
  {ET_RSPQRYCOMBACTION,                         msgQryCombAction},
  {ET_RSPQRYTRANSFERSERIAL,                     msgQryTransferSerial},
  {ET_RSPQRYACCOUNTREGISTER,                    msgQryAccountRegister},
  {ET_RSPERROR,                                 msgError},
  {ET_RTNORDER,                                 msgOrder},
  {ET_RTNTRADE,                                 msgTrade},
  {ET_ERRRTNORDERINSERT,                        msgErrOrderInsert},
  {ET_ERRRTNORDERACTION,                        msgErrOrderAction},
  {ET_RTNINSTRUMENTSTATUS,                      msgInstrumentStatus},
  {ET_RTNBULLETIN,                              msgBulletin},
  {ET_RTNTRADINGNOTICE,                         msgTradingNotice},
  {ET_RTNERRORCONDITIONALORDER,                 msgErrorConditionalOrder},
  {ET_RTNEXECORDER,                             msgExecOrder},
  {ET_ERRRTNEXECORDERINSERT,                    msgErrExecOrderInsert},
  {ET_ERRRTNEXECORDERACTION,                    msgErrExecOrderAction},
  {ET_ERRRTNFORQUOTEINSERT,                     msgErrForQuoteInsert},
  {ET_RTNQUOTE,                                 msgQuote},
  {ET_ERRRTNQUOTEINSERT,                        msgErrQuoteInsert},
  {ET_ERRRTNQUOTEACTION,                        msgErrQuoteAction},
  {ET_RTNFORQUOTERSP,                           msgForQuote},
  {ET_RTNCFMMCTRADINGACCOUNTTOKEN,              msgCFMMCTradingAccountToken},
  {ET_ERRRTNBATCHORDERACTION,                   msgErrBatchOrderAction},
  {ET_RTNOPTIONSELFCLOSE,                       msgOptionSelfClose},
  {ET_ERRRTNOPTIONSELFCLOSEINSERT,              msgErrOptionSelfCloseInsert},
  {ET_ERRRTNOPTIONSELFCLOSEACTION,              msgErrOptionSelfCloseAction},
  {ET_RTNCOMBACTION,                            msgCombAction},
  {ET_ERRRTNCOMBACTIONINSERT,                   msgErrCombActionInsert},
  {ET_RSPQRYCONTRACTBANK,                       msgQryContractBank},
  {ET_RSPQRYPARKEDORDER,                        msgQryParkedOrder},
  {ET_RSPQRYPARKEDORDERACTION,                  msgQryParkedOrderAction},
  {ET_RSPQRYTRADINGNOTICE,                      msgQryTradingNotice},
  {ET_RSPQRYBROKERTRADINGPARAMS,                msgQryBrokerTradingParams},
  {ET_RSPQRYBROKERTRADINGALGOS,                 msgQryBrokerTradingAlgos},
  {ET_RSPQUERYCFMMCTRADINGACCOUNTTOKEN,         msgQueryCFMMCTradingAccountToken},
  {ET_RTNFROMBANKTOFUTUREBYBANK,                msgFromBankToFutureByBank},
  {ET_RTNFROMFUTURETOBANKBYBANK,                msgFromFutureToBankByBank},
  {ET_RTNREPEALFROMBANKTOFUTUREBYBANK,          msgRepealFromBankToFutureByBank},
  {ET_RTNREPEALFROMFUTURETOBANKBYBANK,          msgRepealFromFutureToBankByBank},
  {ET_RTNFROMBANKTOFUTUREBYFUTURE,              msgFromBankToFutureByFuture},
  {ET_RTNFROMFUTURETOBANKBYFUTURE,              msgFromFutureToBankByFuture},
  {ET_RTNREPEALFROMBANKTOFUTUREBYFUTUREMANUAL,  msgRepealFromBankToFutureByFutureManual},
  {ET_RTNREPEALFROMFUTURETOBANKBYFUTUREMANUAL,  msgRepealFromFutureToBankByFutureManual},
  {ET_RTNQUERYBANKBALANCEBYFUTURE,              msgQueryBankBalanceByFuture},
  {ET_ERRRTNBANKTOFUTUREBYFUTURE,               msgErrBankToFutureByFuture},
  {ET_ERRRTNFUTURETOBANKBYFUTURE,               msgErrFutureToBankByFuture},
  {ET_ERRRTNREPEALBANKTOFUTUREBYFUTUREMANUAL,   msgErrRepealBankToFutureByFutureManual},
  {ET_ERRRTNREPEALFUTURETOBANKBYFUTUREMANUAL,   msgErrRepealFutureToBankByFutureManual},
  {ET_ERRRTNQUERYBANKBALANCEBYFUTURE,           msgErrQueryBankBalanceByFuture},
  {ET_RTNREPEALFROMBANKTOFUTUREBYFUTURE,        msgRepealFromBankToFutureByFuture},
  {ET_RTNREPEALFROMFUTURETOBANKBYFUTURE,        msgRepealFromFutureToBankByFuture},
  {ET_RSPFROMBANKTOFUTUREBYFUTURE,              msgRspFromBankToFutureByFuture},
  {ET_RSPFROMFUTURETOBANKBYFUTURE,              msgRspFromFutureToBankByFuture},
  {ET_RSPQUERYBANKACCOUNTMONEYBYFUTURE,         msgQueryBankAccountMoneyByFuture},
  {ET_RTNOPENACCOUNTBYBANK,                     msgOpenAccountByBank},
  {ET_RTNCANCELACCOUNTBYBANK,                   msgCancelAccountByBank},
  {ET_RTNCHANGEACCOUNTBYBANK,                   msgChangeAccountByBank},
  {ET_RSPQRYCLASSIFIEDINSTRUMENT,               msgQryClassifiedInstrument},
  {ET_RSPQRYCOMBPROMOTIONPARAM,                 msgQryCombPromotionParam},
  {ET_RSPQRYRISKSETTLEINVSTPOSITION,            msgQryRiskSettleInvstPosition},
  {ET_RSPQRYRISKSETTLEPRODUCTSTATUS,            msgQryRiskSettleProductStatus},
};

napi_status getTraderMessageValue(napi_env env, const Message *message, napi_value *result) {
  return getMessageValue(env, messageFuncs, message, result);
}