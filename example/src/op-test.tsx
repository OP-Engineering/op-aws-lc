import { ScrollView, StyleSheet, Text, View } from 'react-native';

type TestFunction = () => void | Promise<void>;
type HookFunction = () => void | Promise<void>;

interface Test {
  name: string;
  fn: TestFunction;
  only?: boolean;
}

export interface TestResult {
  name: string;
  passed: boolean;
  errorMessage?: string;
}

export interface DescribeBlock {
  name: string;
  tests: (Test | DescribeBlock)[];
  beforeAllHooks: HookFunction[];
  beforeEachHooks: HookFunction[];
  afterAllHooks: HookFunction[];
  afterEachHooks: HookFunction[];
}

const rootDescribeBlock: DescribeBlock = {
  name: 'root',
  tests: [],
  beforeAllHooks: [],
  beforeEachHooks: [],
  afterAllHooks: [],
  afterEachHooks: [],
};

// @ts-expect-error
module.hot?.dispose(() => {
  rootDescribeBlock.tests = [];
});

let currentDescribeBlock = rootDescribeBlock;

export function describe(name: string, fn: () => void) {
  const parentDescribeBlock = currentDescribeBlock;

  const existingDescribeIndex = parentDescribeBlock.tests.findIndex(
    (test): test is DescribeBlock => !('fn' in test) && test.name === name
  );

  if (existingDescribeIndex !== -1) {
    parentDescribeBlock.tests.splice(existingDescribeIndex, 1);
  }
  const newDescribeBlock: DescribeBlock = {
    name,
    tests: [],
    beforeAllHooks: [],
    beforeEachHooks: [],
    afterAllHooks: [],
    afterEachHooks: [],
  };

  parentDescribeBlock.tests.push(newDescribeBlock);

  currentDescribeBlock = newDescribeBlock;
  fn();
  currentDescribeBlock = parentDescribeBlock;
}

export function it(name: string, fn: TestFunction) {
  currentDescribeBlock.tests.push({ name, fn });
}

export function itOnly(name: string, fn: TestFunction) {
  currentDescribeBlock.tests.push({ name, fn, only: true });
}

export function beforeAll(fn: HookFunction) {
  currentDescribeBlock.beforeAllHooks.push(fn);
}

export function beforeEach(fn: HookFunction) {
  currentDescribeBlock.beforeEachHooks.push(fn);
}

export function afterAll(fn: HookFunction) {
  currentDescribeBlock.afterAllHooks.push(fn);
}

export function afterEach(fn: HookFunction) {
  currentDescribeBlock.afterEachHooks.push(fn);
}

export function expect(value: any) {
  return {
    toBe(expected: any) {
      if (value !== expected) {
        throw new Error(`Expected ${value} to be ${expected}`);
      }
    },
    toEqual(expected: any) {
      if (value != expected) {
        throw new Error(`Expected ${value} to equal ${expected}`);
      }
    },
    not: {
      toBe(expected: any) {
        if (value === expected) {
          throw new Error(`Expected ${value} not to be ${expected}`);
        }
      },
      toEqual(expected: any) {
        if (value == expected) {
          throw new Error(`Expected ${value} not to equal ${expected}`);
        }
      },
    },
    resolves: {
      async toBe(expected: any) {
        const resolvedValue = await value;
        if (resolvedValue !== expected) {
          throw new Error(
            `Expected resolved value ${resolvedValue} to be ${expected}`
          );
        }
      },
      async toEqual(expected: any) {
        const resolvedValue = await value;
        if (resolvedValue != expected) {
          throw new Error(
            `Expected resolved value ${resolvedValue} to equal ${expected}`
          );
        }
      },
    },
    rejects: {
      async toThrow(expected: any) {
        try {
          await value;
          throw new Error(
            `Expected promise to throw ${expected}, but it resolved`
          );
        } catch (error) {
          if (error instanceof Error && error.message !== expected) {
            throw new Error(
              `Expected promise to throw ${expected}, but it threw ${error.message}`
            );
          }
        }
      },
    },
  };
}

async function runDescribeBlock(
  describeBlock: DescribeBlock
): Promise<DescribeBlock> {
  const results: DescribeBlock = {
    ...describeBlock,
    tests: [],
  };

  for (const hook of describeBlock.beforeAllHooks) {
    await hook();
  }

  for (const testOrBlock of describeBlock.tests) {
    if ('fn' in testOrBlock) {
      const { name, fn } = testOrBlock;
      for (const hook of describeBlock.beforeEachHooks) {
        await hook();
      }

      try {
        await fn();
        results.tests.push({ name, passed: true, fn } as Test);
      } catch (error) {
        const errorMessage =
          error instanceof Error ? error.message : String(error);
        results.tests.push({ name, passed: false, errorMessage, fn } as Test);
      }

      for (const hook of describeBlock.afterEachHooks) {
        await hook();
      }
    } else {
      const nestedResults = await runDescribeBlock(testOrBlock);
      results.tests.push(nestedResults);
    }
  }

  for (const hook of describeBlock.afterAllHooks) {
    await hook();
  }

  return results;
}

export async function runTests(): Promise<DescribeBlock> {
  return runDescribeBlock(rootDescribeBlock);
}

export function displayResults(results: DescribeBlock) {
  const renderResults = (describeBlock: DescribeBlock) => {
    return (
      <View key={describeBlock.name} style={styles.describeBlock}>
        {describeBlock.name !== 'root' && (
          <Text style={styles.describeBlockName}>{describeBlock.name}</Text>
        )}

        {describeBlock.tests.map((testOrBlock, index) => {
          if ('fn' in testOrBlock) {
            const { name, passed, errorMessage } =
              testOrBlock as unknown as TestResult;
            return (
              <View key={index} style={styles.resultRow}>
                <View
                  style={[
                    styles.resultIndicator,
                    { backgroundColor: passed ? '#4CAF50' : '#f44336' },
                  ]}
                />
                <View style={styles.resultContent}>
                  <Text style={styles.text}>{name}</Text>
                  {!passed && (
                    <Text style={styles.errorMessage}>{errorMessage}</Text>
                  )}
                </View>
              </View>
            );
          } else {
            return renderResults(testOrBlock as DescribeBlock);
          }
        })}
      </View>
    );
  };

  const allResults = (describeBlock: DescribeBlock): TestResult[] => {
    const results: TestResult[] = [];
    for (const testOrBlock of describeBlock.tests) {
      if ('fn' in testOrBlock) {
        const test = testOrBlock as Test;
        results.push({ name: test.name, passed: true });
      } else {
        results.push(...allResults(testOrBlock as DescribeBlock));
      }
    }
    return results;
  };

  const allTestResults = allResults(results);

  return (
    <View style={styles.container}>
      <View style={styles.summary}>
        <View
          style={[
            styles.indicator,
            {
              backgroundColor: allTestResults.every((r) => r.passed)
                ? '#4CAF50'
                : '#f44336',
            },
          ]}
        />
        <Text style={styles.text}>
          {allTestResults.filter((r) => r.passed).length} of{' '}
          {allTestResults.length} tests passed
        </Text>
      </View>
      <ScrollView style={styles.scrollView}>
        {renderResults(results)}
      </ScrollView>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    padding: 10,
  },
  text: {
    color: 'white',
  },
  summary: {
    marginBottom: 20,
    flexDirection: 'row',
    alignItems: 'center',
    gap: 10,
  },
  indicator: {
    width: 12,
    height: 12,
    borderRadius: 6,
  },
  scrollView: {
    maxHeight: 400,
  },
  describeBlock: {
    marginBottom: 16,
  },
  describeBlockName: {
    fontWeight: 'bold',
    marginBottom: 2,
    color: 'white',
  },
  resultRow: {
    padding: 12,
    borderBottomWidth: 1,
    borderBottomColor: '#444',
    flexDirection: 'row',
    alignItems: 'center',
    gap: 10,
  },
  resultIndicator: {
    width: 8,
    height: 8,
    borderRadius: 4,
  },
  resultContent: {
    flex: 1,
  },
  errorMessage: {
    color: '#f44336',
    fontSize: 14,
    marginTop: 4,
  },
});
