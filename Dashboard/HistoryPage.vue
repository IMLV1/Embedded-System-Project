<template>
  <div class="container">
    <!-- Header -->
    <header class="header">
      <div class="logo">💊 <span>PillCare</span></div>
    </header>

    <!-- Log & History Section -->
    <section class="card gradient">
      <h2>📊 Log & History</h2>
      <p>Complete medication intake history and analytics</p>
    </section>

    <!-- Filters Section -->
    <section class="card">
      <h3>⚙️ Filters</h3>
      <div class="filters">
        <div>
          <label>Date Range</label>
          <select v-model="dateRange">
            <option>Last 7 days</option>
            <option>Last 30 days</option>
          </select>
        </div>
        <div>
          <label>Status Filter</label>
          <select v-model="statusFilter">
            <option>All Status</option>
            <option>Taken</option>
            <option>Missed</option>
            <option>Pending</option>
          </select>
        </div>
      </div>
    </section>

    <!-- Charts Section -->
    <!-- <div class="charts"> -->
      <section class="card chart">
        <h4>💊 Medication Compliance</h4>
        <canvas ref="pieChart"></canvas>
      </section>
    <!-- </div> -->

    <!-- Detailed Log -->
    <section class="Table-Datail">
      <h3>📋 Detailed Log</h3>
      <table>
        <thead>
          <tr>
            <th>Date</th>
            <th>Time</th>
            <th>Pill Slot</th>
            <th>Pill Name</th>
            <th>Status</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="(item,index) in filteredData" :key="index">
            <td>{{ item.date }}</td>
            <td>{{ item.time }}</td>
            <td><span class="slot">{{ item.slot }}</span></td>
            <td>{{ item.name }}</td>
            <td>
              <span :class="['status', item.status?.toLowerCase()]">{{ item.status }}</span>
            </td>
          </tr>
        </tbody>
      </table>
    </section>
  </div>
</template>

<script>
  export default {
  data() {
    return {
      dateRange: "Last 7 days",
      statusFilter: "All Status",
      history: [],
      stats: { taken: 0, missed: 0, pending: 0 },
      pieChart: null,
      barChart: null,
    };
  },

  computed: {
    filteredData() {
      if (this.statusFilter === "All Status") return this.history;
      return this.history.filter(i => i.status === this.statusFilter);
    },
  },

  methods: {
    async ensureChartJsReady() {
      if (window.Chart) return true;

      return new Promise((resolve) => {
        const script = document.createElement("script");
        script.src = "https://cdn.jsdelivr.net/npm/chart.js@4.4.1/dist/chart.umd.min.js";
        script.onload = () => {
          console.log("✅ Chart.js loaded.");
          resolve(true);
        };
        document.head.appendChild(script);
      });
    },

    async initCharts() {
      // 🟢 รอ Chart.js โหลดก่อน
      await this.ensureChartJsReady();
      const Chart = window.Chart;
      console.log("✅ Chart.js ready, creating charts...");

      this.$nextTick(() => {
        // Pie Chart
        const pieCtx = this.$refs.pieChart.getContext("2d");
        this.pieChart = new Chart(pieCtx, {
          type: "pie",
          data: {
            labels: ["Taken", "Missed", "Pending"],
            datasets: [
              {
                data: [this.stats.taken, this.stats.missed, this.stats.pending],
                backgroundColor: ["#4CAF50", "#F44336", "#FFC107"],
              },
            ],
          },
          options: {
            aspectRatio: 1.6,
            plugins: { legend: { position: "bottom" } },
          },
        });

        // Bar Chart
        const barCtx = this.$refs.barChart.getContext("2d");
        this.barChart = new Chart(barCtx, {
          type: "bar",
          data: {
            labels: ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"],
            datasets: [
              { label: "Missed", data: [1, 0, 1, 0, 0, 0, 0], backgroundColor: "#F44336" },
              { label: "Taken", data: [4, 5, 4, 5, 4, 5, 5], backgroundColor: "#4CAF50" },
            ],
          },
          options: {
            responsive: true,
            maintainAspectRatio: true,
            aspectRatio: 2.2,
            scales: {
              y: { beginAtZero: true, ticks: { stepSize: 1 }, suggestedMax: 6 },
            },
            plugins: { legend: { position: "bottom" } },
          },
        });
      });
    },

    updateCharts(stats) {
      if (!this.pieChart || !this.barChart) return;
      setTimeout(() => {
        try {
          this.pieChart.data.datasets[0].data = [
            stats.taken || 0,
            stats.missed || 0,
            stats.pending || 0,
          ];
          this.pieChart.update();

          const missed = stats.weeklyTrend?.missed || [0, 0, 0, 0, 0, 0, 0];
          const taken = stats.weeklyTrend?.taken || [0, 0, 0, 0, 0, 0, 0];
          this.barChart.data.datasets[0].data = missed;
          this.barChart.data.datasets[1].data = taken;
          this.barChart.update();

          console.log("✅ Charts updated successfully");
        } catch (err) {
          console.warn("⚠️ Chart update error:", err.message);
        }
      }, 200);
    },
  },

  mounted() {
    console.log("🟢 History UI ready");
    this.initCharts();

    this.$watch(
      "msg",
      (newMsg) => {
        console.log("📩 Full msg received:", newMsg);
        const payload =
          newMsg?.payload?.history || newMsg?.payload?.stats
            ? newMsg.payload
            : newMsg?.payload?.payload || {};

        if (payload.history && Array.isArray(payload.history)) {
          this.history = [...payload.history];
        }

        if (payload.stats) {
          this.stats = payload.stats;
          this.updateCharts(payload.stats);
        }
      },
      { immediate: true, deep: true }
    );
  },
};
</script>

<style scoped>
  @import url("https://fonts.googleapis.com/css2?family=Inter:wght@400;600&display=swap");

  * {
    font-family: "Inter", sans-serif;
  }

  .container {
    width: 100%;
    max-width: 1400px;
    margin: 0 auto;
    background: #ffffff;
    padding: 30px 40px;
    box-sizing: border-box;
    color: #333;
  }

  .header {
    display: flex;
    align-items: center;
    margin-bottom: 15px;
  }

  .logo {
    font-size: 35px;
    font-weight: bold;
    color: #2ecc71;
    display: flex;
    align-items: center;
    gap: 5px;
  }

  .card {
    background: #fff;
    border-radius: 12px;
    padding: 20px 25px;
    margin: 15px 0;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.05);
  }

  .gradient {
    background: linear-gradient(to right, #e8f8f5, #f9fdf8);
  }

  .filters {
    display: flex;
    gap: 30px;
    margin-top: 10px;
  }

  .filters select {
    width: 180px;
    padding: 8px;
    border-radius: 8px;
    border: 1px solid #ddd;
  }

  .charts {
    display: flex;
    gap: 25px;
  }

  canvas {
    width: 100%;
    height: auto;
    max-height: 340px;
  }

  .Table-Datail {
    background: #fff;
    border-radius: 12px;
    padding: 20px;
    margin-top: 20px;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.05);
  }

  table {
    width: 100%;
    border-collapse: collapse;
    margin-top: 10px;
  }

  th,
  td {
    text-align: left;
    padding: 10px;
    border-bottom: 1px solid #eee;
  }

  .slot {
    background: #f3f3f3;
    border-radius: 6px;
    padding: 3px 8px;
    font-weight: 500;
  }

  .status {
    border-radius: 8px;
    padding: 4px 10px;
    color: white;
    font-weight: bold;
  }

  .status.taken {
    background: #4caf50;
  }

  .status.missed {
    background: #f44336;
  }

  .status.pending {
    background: #ffc107;
    color: #333;
  }

  body,
  .nr-dashboard-theme,
  .nr-dashboard-cardcontainer,
  html,
  .v-application,
  .v-main,
  .flowfuse-dashboard {
    background-color: #ffffff !important;
  }
</style>