///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HLOperations.h                                                            //
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
// This file is distributed under the University of Illinois Open Source     //
// License. See LICENSE.TXT for details.                                     //
//                                                                           //
// Implentation of High Level DXIL operations.                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

namespace llvm {
class Module;
class Function;
class CallInst;
class Argument;
class StringRef;
class FunctionType;
}

namespace hlsl {

enum class HLOpcodeGroup {
  NotHL,
  HLExtIntrinsic,
  HLIntrinsic,
  HLCast,
  HLInit,
  HLBinOp,
  HLUnOp,
  HLSubscript,
  HLMatLoadStore,
  HLSelect,
  HLCreateHandle,
  NumOfHLOps
};

enum class HLBinaryOpcode {
  Invalid,
  Mul,
  Div,
  Rem,
  Add,
  Sub,
  Shl,
  Shr,
  LT,
  GT,
  LE,
  GE,
  EQ,
  NE,
  And,
  Xor,
  Or,
  LAnd,
  LOr,
  UDiv,
  URem,
  UShr,
  ULT,
  UGT,
  ULE,
  UGE,
  NumOfBO,
};

enum class HLUnaryOpcode {
  Invalid,
  PostInc,
  PostDec,
  PreInc,
  PreDec,
  Plus,
  Minus,
  Not,
  LNot,
  NumOfUO,
};

enum class HLSubscriptOpcode {
  DefaultSubscript,
  ColMatSubscript,
  RowMatSubscript,
  ColMatElement,
  RowMatElement,
  DoubleSubscript,
  CBufferSubscript,
  VectorSubscript,   // Only for bool vector, other vector type will use GEP directly.
};

enum class HLCastOpcode {
  DefaultCast,
  UnsignedUnsignedCast,
  FromUnsignedCast,
  ToUnsignedCast,
  ColMatrixToVecCast,
  RowMatrixToVecCast,
  ColMatrixToRowMatrix,
  RowMatrixToColMatrix,
  HandleToResCast,
};

enum class HLMatLoadStoreOpcode {
  ColMatLoad,
  ColMatStore,
  RowMatLoad,
  RowMatStore,
};

extern const char * const HLPrefix;

HLOpcodeGroup GetHLOpcodeGroup(llvm::Function *F);
HLOpcodeGroup GetHLOpcodeGroupByName(const llvm::Function *F);
llvm::StringRef GetHLOpcodeGroupNameByAttr(llvm::Function *F);
llvm::StringRef GetHLLowerStrategy(llvm::Function *F);
unsigned  GetHLOpcode(const llvm::CallInst *CI);
unsigned  GetRowMajorOpcode(HLOpcodeGroup group, unsigned opcode);
void SetHLLowerStrategy(llvm::Function *F, llvm::StringRef S);

// For intrinsic opcode.
bool HasUnsignedOpcode(unsigned opcode);
unsigned GetUnsignedOpcode(unsigned opcode);
// For HLBinaryOpcode.
bool HasUnsignedOpcode(HLBinaryOpcode opcode);
HLBinaryOpcode GetUnsignedOpcode(HLBinaryOpcode opcode);

llvm::StringRef GetHLOpcodeGroupName(HLOpcodeGroup op);

namespace HLOperandIndex {
// Opcode parameter.
const unsigned kOpcodeIdx = 0;

// Matrix store.
const unsigned kMatStoreDstPtrOpIdx = 1;
const unsigned kMatStoreValOpIdx = 2;

// Matrix load.
const unsigned kMatLoadPtrOpIdx = 1;

// Normal subscipts.
const unsigned kSubscriptObjectOpIdx = 1;
const unsigned kSubscriptIndexOpIdx = 2;

// Double subscripts.
const unsigned kDoubleSubscriptMipLevelOpIdx = 3;

// Matrix subscripts.
const unsigned kMatSubscriptMatOpIdx = 1;
const unsigned kMatSubscriptSubOpIdx = 2;

// Matrix init.
const unsigned kMatArrayInitMatOpIdx = 1;
const unsigned kMatArrayInitFirstArgOpIdx = 2;

// Array Init.
const unsigned kArrayInitPtrOpIdx = 1;
const unsigned kArrayInitFirstArgOpIdx = 2;

// Normal Init.
const unsigned kInitFirstArgOpIdx = 1;

// Unary operators.
const unsigned kUnaryOpSrc0Idx = 1;

// Binary operators.
const unsigned kBinaryOpSrc0Idx = 1;
const unsigned kBinaryOpSrc1Idx = 2;

// Trinary operators.
const unsigned kTrinaryOpSrc0Idx = 1;
const unsigned kTrinaryOpSrc1Idx = 2;
const unsigned kTrinaryOpSrc2Idx = 3;

// Interlocked.
const unsigned kInterlockedDestOpIndex = 1;
const unsigned kInterlockedValueOpIndex = 2;
const unsigned kInterlockedOriginalValueOpIndex = 3;

// InterlockedCompareExchange.
const unsigned kInterlockedCmpDestOpIndex = 1;
const unsigned kInterlockedCmpCompareValueOpIndex = 2;
const unsigned kInterlockedCmpValueOpIndex = 3;
const unsigned kInterlockedCmpOriginalValueOpIndex = 4;

// Lerp.
const unsigned kLerpOpXIdx = 1;
const unsigned kLerpOpYIdx = 2;
const unsigned kLerpOpSIdx = 3;

// ProcessTessFactorIsoline.
const unsigned kProcessTessFactorRawDetailFactor = 1;
const unsigned kProcessTessFactorRawDensityFactor = 2;
const unsigned kProcessTessFactorRoundedDetailFactor = 3;
const unsigned kProcessTessFactorRoundedDensityFactor = 4;

// ProcessTessFactor.
const unsigned kProcessTessFactorRawEdgeFactor = 1;
const unsigned kProcessTessFactorInsideScale = 2;
const unsigned kProcessTessFactorRoundedEdgeFactor = 3;
const unsigned kProcessTessFactorRoundedInsideFactor = 4;
const unsigned kProcessTessFactorUnRoundedInsideFactor = 5;

// Reflect.
const unsigned kReflectOpIIdx = 1;
const unsigned kReflectOpNIdx = 2;

// Refract
const unsigned kRefractOpIIdx = 1;
const unsigned kRefractOpNIdx = 2;
const unsigned kRefractOpEtaIdx = 3;

// SmoothStep.
const unsigned kSmoothStepOpMinIdx = 1;
const unsigned kSmoothStepOpMaxIdx = 2;
const unsigned kSmoothStepOpXIdx = 3;

// Clamp
const unsigned kClampOpXIdx = 1;
const unsigned kClampOpMinIdx = 2;
const unsigned kClampOpMaxIdx = 3;


// Object functions.
const unsigned kHandleOpIdx = 1;
// Store.
const unsigned kStoreOffsetOpIdx = 2;
const unsigned kStoreValOpIdx = 3;
// Load.
const unsigned kBufLoadAddrOpIdx = 2;
const unsigned kBufLoadStatusOpIdx = 3;
const unsigned kRWTexLoadStatusOpIdx = 3;
const unsigned kTexLoadOffsetOpIdx = 3;
const unsigned kTexLoadStatusOpIdx = 4;
// Load for Texture2DMS
const unsigned kTex2DMSLoadSampleIdxOpIdx = 3;
const unsigned kTex2DMSLoadOffsetOpIdx = 4;
const unsigned kTex2DMSLoadStatusOpIdx = 5;
// mips.Operator.
const unsigned kMipLoadAddrOpIdx = 3;
const unsigned kMipLoadOffsetOpIdx = 4;
const unsigned kMipLoadStatusOpIdx = 5;

// Sample.
const unsigned kSampleSamplerArgIndex = 2;
const unsigned kSampleCoordArgIndex = 3;
const unsigned kSampleOffsetArgIndex = 4;
const unsigned kSampleClampArgIndex = 5;
const unsigned kSampleStatusArgIndex = 6;

// SampleG.
const unsigned kSampleGDDXArgIndex = 4;
const unsigned kSampleGDDYArgIndex = 5;
const unsigned kSampleGOffsetArgIndex = 6;
const unsigned kSampleGClampArgIndex = 7;
const unsigned kSampleGStatusArgIndex = 8;

// SampleCmp.
const unsigned kSampleCmpCmpValArgIndex = 4;
const unsigned kSampleCmpOffsetArgIndex = 5;
const unsigned kSampleCmpClampArgIndex = 6;
const unsigned kSampleCmpStatusArgIndex = 7;

// SampleBias.
const unsigned kSampleBBiasArgIndex = 4;
const unsigned kSampleBOffsetArgIndex = 5;
const unsigned kSampleBClampArgIndex = 6;
const unsigned kSampleBStatusArgIndex = 7;

// SampleLevel.
const unsigned kSampleLLevelArgIndex = 4;
const unsigned kSampleLOffsetArgIndex = 5;
const unsigned kSampleLStatusArgIndex = 6;

// SampleCmpLevelZero.
const unsigned kSampleCmpLZCmpValArgIndex = 4;
const unsigned kSampleCmpLZOffsetArgIndex = 5;
const unsigned kSampleCmpLZStatusArgIndex = 6;

// Gather.
const unsigned kGatherSamplerArgIndex = 2;
const unsigned kGatherCoordArgIndex = 3;
const unsigned kGatherOffsetArgIndex = 4;
const unsigned kGatherStatusArgIndex = 5;
const unsigned kGatherSampleOffsetArgIndex = 5;
const unsigned kGatherStatusWithSampleOffsetArgIndex = 8;

// GatherCmp.
const unsigned kGatherCmpCmpValArgIndex = 4;
const unsigned kGatherCmpOffsetArgIndex = 5;
const unsigned kGatherCmpStatusArgIndex = 6;
const unsigned kGatherCmpSampleOffsetArgIndex = 6;
const unsigned kGatherCmpStatusWithSampleOffsetArgIndex = 9;

// WriteSamplerFeedback.
const unsigned kWriteSamplerFeedbackSampledArgIndex = 2;
const unsigned kWriteSamplerFeedbackSamplerArgIndex = 3;
const unsigned kWriteSamplerFeedbackCoordArgIndex = 4;
const unsigned kWriteSamplerFeedbackBiasOrLodArgIndex = 5;
const unsigned kWriteSamplerFeedbackDdxArgIndex = 5;
const unsigned kWriteSamplerFeedbackDdyArgIndex = 6;

// StreamAppend.
const unsigned kStreamAppendStreamOpIndex = 1;
const unsigned kStreamAppendDataOpIndex = 2;

// Append.
const unsigned kAppendValOpIndex = 2;

// Interlocked.
const unsigned kObjectInterlockedDestOpIndex = 2;
const unsigned kObjectInterlockedValueOpIndex = 3;
const unsigned kObjectInterlockedOriginalValueOpIndex = 4;

// InterlockedCompareExchange.
const unsigned kObjectInterlockedCmpDestOpIndex = 2;
const unsigned kObjectInterlockedCmpCompareValueOpIndex = 3;
const unsigned kObjectInterlockedCmpValueOpIndex = 4;
const unsigned kObjectInterlockedCmpOriginalValueOpIndex = 5;

// GetSamplePosition.
const unsigned kGetSamplePositionSampleIdxOpIndex = 2;

// GetDimensions.
const unsigned kGetDimensionsMipLevelOpIndex = 2;
const unsigned kGetDimensionsMipWidthOpIndex = 3;
const unsigned kGetDimensionsNoMipWidthOpIndex = 2;

// WaveAllEqual.
const unsigned kWaveAllEqualValueOpIdx = 1;

// CreateHandle.
const unsigned kCreateHandleResourceOpIdx = 1;
const unsigned kCreateHandleIndexOpIdx = 2; // Only for array of cbuffer.

// TraceRay.
const unsigned kTraceRayRayDescOpIdx = 7;
const unsigned kTraceRayPayLoadOpIdx = 8;

// TraceRayInline.
const unsigned kTraceRayInlineRayDescOpIdx = 5;

// ReportIntersection.
const unsigned kReportIntersectionAttributeOpIdx = 3;

// DispatchMesh
const unsigned kDispatchMeshOpThreadX = 1;
const unsigned kDispatchMeshOpThreadY = 2;
const unsigned kDispatchMeshOpThreadZ = 3;
const unsigned kDispatchMeshOpPayload = 4;

} // namespace HLOperandIndex

llvm::Function *GetOrCreateHLFunction(llvm::Module &M,
                                      llvm::FunctionType *funcTy,
                                      HLOpcodeGroup group, unsigned opcode);
llvm::Function *GetOrCreateHLFunction(llvm::Module &M,
                                      llvm::FunctionType *funcTy,
                                      HLOpcodeGroup group,
                                      llvm::StringRef *groupName,
                                      llvm::StringRef *fnName,
                                      unsigned opcode);

llvm::Function *GetOrCreateHLFunctionWithBody(llvm::Module &M,
                                              llvm::FunctionType *funcTy,
                                              HLOpcodeGroup group,
                                              unsigned opcode,
                                              llvm::StringRef name);
} // namespace hlsl